/*
 * vzsdk
 * Copyright 2013 - 2016, Vzenith Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "vzsdk/session.h"
#include "base/logging.h"

namespace vzsdk {

uint32 Session::unequal_session_id_ = 0X01;

Session::Session(Thread* async_thread, int _session_id)
  : async_thread_(async_thread),
    connect_id_(0) {
  if (_session_id == 0)
    session_id_ = ++unequal_session_id_;
  else
    session_id_ = _session_id;
}

Session::~Session() {
  // LOG(LS_WARNING) << "Session destory ... ...";
}

void Session::OnMessage(Message* msg) {
  ASSERT(async_thread_ == Thread::Current());
  WriteHeartbeat();
  AsyncWaitNextHeadrtbeatEvent();
}

bool Session::Start(SocketAddress &remote_address, uint32 connect_id) {
  connect_id_ = connect_id;
  ASSERT(async_thread_ == Thread::Current());
  ASSERT(async_socket_ == NULL);
  if(async_socket_ != NULL) {
    return false;
  }
  InitSocket();
  async_socket_->Connect(remote_address);
  return true;
}

bool Session::Stop() {
  ASSERT(async_thread_ == Thread::Current());
  UinitSocket();
  if(async_socket_ && async_socket_->GetState() != Socket::CS_CLOSED) {
    async_socket_->Close();
  }
  return true;
}

void Session::InitSocket() {
  async_socket_.reset(
    async_thread_->socketserver()->CreateAsyncSocket(SOCK_STREAM));
  ASSERT(async_thread_ != NULL);
  async_socket_->SignalReadEvent.connect(this, &Session::OnReadEvent);
  async_socket_->SignalWriteEvent.connect(this, &Session::OnWriteEvent);
  async_socket_->SignalConnectEvent.connect(this, &Session::OnConnectEvent);
  async_socket_->SignalCloseEvent.connect(this, &Session::OnCloseEvent);
}

void Session::UinitSocket() {
  ASSERT(async_thread_ == Thread::Current());
  if(async_socket_ != NULL && async_socket_->GetState() != Socket::CS_CLOSED) {
    async_socket_->SignalReadEvent.disconnect_all();
    async_socket_->SignalWriteEvent.disconnect_all();
    async_socket_->SignalConnectEvent.disconnect_all();
    async_socket_->SignalCloseEvent.disconnect_all();
  }
}

void Session::OnReadEvent(AsyncSocket::Ptr async_socket) {
  ASSERT(async_socket_ == async_socket);
  while(1) {
    int recv_size = async_socket_->Recv(temp_buffer_, DEFAULT_TEMP_BUFFER_SIZE);
    if(recv_size == SOCKET_ERROR) {
      // LOG(LS_INFO) << "SOCKET_ERROR = " << async_socket_->GetError();
      break;
    } else {
      read_buffer_.WriteBytes(temp_buffer_, recv_size);
      ProcessInput();
    }
  }
}

void Session::OnWriteEvent(AsyncSocket::Ptr async_socket) {
  ASSERT(async_socket_ == async_socket);
  TryToWriteData();
}

void Session::OnConnectEvent(AsyncSocket::Ptr async_socket) {
  ASSERT(async_socket_ == async_socket);
  // Connect the remote server succeed
  SignalSessionConnectedEvent(shared_from_this(), connect_id_);
  AsyncWaitNextHeadrtbeatEvent();
}

void Session::OnCloseEvent(AsyncSocket::Ptr async_socket, int code) {
  ASSERT(async_socket_ == async_socket);
  UinitSocket();
  SignalSessionCloseEvent(shared_from_this(), code, connect_id_);
}

void Session::ProcessInput() {
  ASSERT(read_buffer_.Length() != 0);
  while(1) {
    std::size_t data_size = read_buffer_.Length();
    if(data_size == 0) {
      break;
    }
    const char *data = read_buffer_.Data();
    int process_size = ProcessPacketProtocol(data, data_size);
    if(process_size == PARSE_PACKET_NEED_MORE_DATA) {
      break;
    } else {
      read_buffer_.Consume(process_size);
    }
  }
}

int Session::ProcessPacketProtocol(const char *data, std::size_t data_size) {
  ASSERT(data_size != 0);
  std::size_t offset = 0;
  uint8 packet_type = 0;
  uint8 packet_number = 0;
  uint32 packet_size = 0;
  while(offset < data_size) {
    if((data_size - offset) < HEADER_SIZE) {
      return PARSE_PACKET_NEED_MORE_DATA;
    }
    // 1. Read VZ
    if(data[offset] == HEADER_V && data[offset + 1] == HEADER_Z) {
      offset += 2;
    } else {
      return PARSE_PACKET_NEED_MORE_DATA;
    }
    // 2. Read Packet type
    packet_type = data[offset];
    offset++;
    // 3. Read packet number
    packet_number = data[offset];
    offset++;
    // 4. Read packet size
    memcpy((void *)&packet_size, (const void *)(data + offset), sizeof(uint32));
    packet_size = ntohl(packet_size);
    offset += 4;
    // 5. Read packet data
    if(data_size - offset >= packet_size) {
      OnHandleInputPacket(data + offset, packet_size, packet_type);
      offset += packet_size;
    } else {
      return PARSE_PACKET_NEED_MORE_DATA;
    }
  }
  return offset;
}

void Session::OnHandleInputPacket(const char *data,
                                  uint32 data_size,
                                  uint8 packet_type) {
  if(packet_type == PACKET_TYPE_HEARTBEAT) {
    ASSERT(data_size == 0);
    // WriteHeartbeat();
  } else if (packet_type == PACKET_TYPE_JSON) {
    SignalSessionPacketEvent(shared_from_this(), data, data_size, packet_type);
  } else if (packet_type == PACKET_TYPE_BINARY) {
    SignalSessionPacketEvent(shared_from_this(), data, data_size, packet_type);
  } else {
    LOG(LS_ERROR) << "Packet type error";
  }
}

void Session::WriteHeartbeat() {
  AsyncWrite(NULL, 0, PACKET_TYPE_HEARTBEAT);
}

void Session::AsyncWaitNextHeadrtbeatEvent() {
  async_thread_->PostDelayed(DEFAULT_HEADBEAT_TIMEOUT, this);
}

void Session::AsyncWrite(const char *data,
                         uint32 data_size,
                         uint8 packet_type) {
  ASSERT(async_thread_ == Thread::Current());
  write_buffer_.WriteUInt8(HEADER_V);
  write_buffer_.WriteUInt8(HEADER_Z);
  write_buffer_.WriteUInt8(packet_type);
  write_buffer_.WriteUInt8(0);
  write_buffer_.WriteUInt32(data_size);
  if(data != NULL) {
    write_buffer_.WriteBytes(data, data_size);
  }
  TryToWriteData();
}

void Session::TryToWriteData() {
  if(async_socket_ == NULL
      || async_socket_->GetState() != Socket::CS_CONNECTED
      || write_buffer_.Length() == 0) {
    return;
  }
  const char *data = write_buffer_.Data();
  std::size_t data_size = write_buffer_.Length();
  int send_size = async_socket_->Send((const void *)data, data_size);
  if(send_size != SOCKET_ERROR) {
    LOG(LS_INFO) << "send size = " << send_size;
    write_buffer_.Consume(send_size);
  }
}

}
