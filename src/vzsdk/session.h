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

#ifndef SRC_HSHA_SESSION_H_
#define SRC_HSHA_SESSION_H_

#include "base/noncopyable.h"
#include "base/sigslot.h"
#include "base/asyncsocket.h"
#include "event/thread.h"
#include "base/bytebuffer.h"
#include "base/shared_ptr.h"

namespace vzsdk {

const uint8 PACKET_TYPE_HEARTBEAT      = 0X01;
const uint8 PACKET_TYPE_BINARY         = 0X02;
const uint8 PACKET_TYPE_JSON           = 0X00;

class Session : public noncopyable,
  public sigslot::has_slots<>,
  public boost::enable_shared_from_this<Session>,
  public MessageHandler {
 public:
   Session(Thread* async_thread, int _session_id = 0);
  virtual ~Session();

  typedef boost::shared_ptr<Session> Ptr;

  virtual void OnMessage(Message* msg);
  // ready to read
  sigslot::signal4<Session::Ptr,
          const char *,
          uint32,
          uint8> SignalSessionPacketEvent;
  // connected
  sigslot::signal2<Session::Ptr, uint32> SignalSessionConnectedEvent;
  // closed
  sigslot::signal3<Session::Ptr, int, uint32> SignalSessionCloseEvent;

  Socket::ConnState GetState() const {
    if(async_socket_) {
      return async_socket_->GetState();
    }
    return Socket::CS_CLOSED;
  }
  // Allown only called by async thread
  bool Start(SocketAddress &remote_address, uint32 connect_id = 0);
  // Allown only called by async thread
  bool Stop();
  // Allown only called by async thread
  void AsyncWrite(const char *data,
                  uint32 data_size,
                  uint8 packet_type = PACKET_TYPE_JSON);
  //
  const uint32 session_id() const {
    return session_id_;
  }
 public:
  void OnReadEvent(AsyncSocket::Ptr async_socket);
  void OnWriteEvent(AsyncSocket::Ptr async_socket);
  void OnConnectEvent(AsyncSocket::Ptr async_socket);
  void OnCloseEvent(AsyncSocket::Ptr async_socket, int code);
 private:
  void InitSocket();
  void UinitSocket();
  void ProcessInput();
  int ProcessPacketProtocol(const char *data, 
	  std::size_t data_size,
	  std::size_t *read_size);
  void OnHandleInputPacket(const char *data,
                           uint32 data_size,
                           uint8 packet_type);
  void WriteHeartbeat();
  void AsyncWaitNextHeadrtbeatEvent();
 private:
  void TryToWriteData();
 private:
  static const int DEFAULT_TEMP_BUFFER_SIZE     = 1024;
  static const int PARSE_PACKET_FORMAT_ERROR    = -1;
  static const int PARSE_PACKET_NEED_MORE_DATA  = 0;
  static const int HEADER_SIZE                  = 8;
  static const uint8 HEADER_V                   = 'V';
  static const uint8 HEADER_Z                   = 'Z';
  static const int DEFAULT_HEADBEAT_TIMEOUT     = 2 * 1000;

  boost::shared_ptr<AsyncSocket> async_socket_;
  Thread *async_thread_;
  char temp_buffer_[DEFAULT_TEMP_BUFFER_SIZE];
  ByteBuffer read_buffer_;
  ByteBuffer write_buffer_;
  static uint32 unequal_session_id_;
  uint32 session_id_;

  // The request id
  uint32 connect_id_;

  uint32 ms_start_;
};

}

#endif // SRC_HSHA_SESSION_H_