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

#include "vzsdk/sessionmanager.h"
#include "base/logging.h"
#include "vzsdk/internalmessage.h"
#include "vzsdk/queuelayer.h"

namespace vzsdk {

SessionManager::SessionManager(QueueLayer *queue_layer)
  : queue_layer_(queue_layer) {
}

SessionManager::~SessionManager() {
  LOG(LS_INFO) << "Destory session manager";
}

void SessionManager::OnMessage(Message *msg) {
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  switch (stanza->stanza_type()) {
  case vzsdk::REQ_CONNECT_SERVER:
    OnConnectMessage(msg);
    break;
  case vzsdk::REQ_DISCONNECT_SERVER:
    OnDisconnectMessage(msg);
    break;
  case vzsdk::REQ_SEND_REQUESTION:
    OnRequestMessage(msg);
    break;
  default:
    ASSERT(false);
    break;
  }
}

void SessionManager::Post(uint32 task_id, MessageData::Ptr pdata) {
  async_thread_->Post(this, task_id, pdata);
}

void SessionManager::OnConnectMessage(Message *msg) {
  ReqConnectData *req_connect_data =
    static_cast<ReqConnectData *>(msg->pdata.get());
  Session::Ptr session(new Session(async_thread_.get()));
  bool add_res = AddSession(session);
  ASSERT(add_res);
  session->Start(req_connect_data->address(), msg->message_id);
}

void SessionManager::OnDisconnectMessage(Message *msg) {
}

void SessionManager::OnRequestMessage(Message *msg) {
}

bool SessionManager::Start() {
  return InitAsyncThread();
}

bool SessionManager::Stop() {
  for(std::map<uint32, Session::Ptr>::iterator iter = async_sessions_.begin();
      iter != async_sessions_.end(); iter++) {
    if(iter->second->GetState() == Socket::CS_CONNECTED) {
      iter->second->Stop();
    }
  }
  async_sessions_.clear();
  // TODO(guangleihe) Needs to make sure the complete stop thread process
  async_thread_->Stop();
  return true;
}

bool SessionManager::InitAsyncThread() {
  if(async_thread_) {
    ASSERT(false);
    return false;
  }
  async_thread_.reset(new Thread());
  return async_thread_->Start();
}

void SessionManager::BindSessionSignal(Session::Ptr session) {
  session->SignalSessionCloseEvent.connect(
    this, &SessionManager::OnSessionCloseEvent);
  session->SignalSessionConnectedEvent.connect(
    this, &SessionManager::OnSessionConnectedEvent);
  session->SignalSessionPacketEvent.connect(
    this, &SessionManager::OnSessionPacketEvent);
}

void SessionManager::UnbindSessionSignal(Session::Ptr session) {
  session->SignalSessionCloseEvent.disconnect_all();
  session->SignalSessionConnectedEvent.disconnect_all();
  session->SignalSessionPacketEvent.disconnect_all();
}

bool SessionManager::AddSession(Session::Ptr session) {
  std::map<uint32, Session::Ptr>::iterator iter =
    async_sessions_.find(session->session_id());
  if(iter != async_sessions_.end()) {
    return false;
  }

  BindSessionSignal(session);
  async_sessions_.insert(std::pair<int, Session::Ptr>(
                           session->session_id(),
                           session));
  return true;
}

bool SessionManager::RemoveSession(uint32 session_id) {
  std::map<uint32, Session::Ptr>::iterator iter =
    async_sessions_.find(session_id);
  if(iter == async_sessions_.end()) {
    return false;
  }
  UnbindSessionSignal(iter->second);
  async_sessions_.erase(iter);
  return true;
}

void SessionManager::OnSessionPacketEvent(
  vzsdk::Session::Ptr session,
  const char *data,
  uint32 data_size,
  uint8 packet_type) {
  //
  LOG(LS_INFO).write(data, data_size);
}

void SessionManager::OnSessionConnectedEvent(
  vzsdk::Session::Ptr session,
  uint32 connect_id) {
  //
  MessageData::Ptr stanza(new Stanza(RES_CONNECTED_EVENT,
                                     session->session_id()));
  queue_layer_->Post(connect_id, stanza);
  LOG(LS_INFO) << "remote connect successfull";
}

void SessionManager::OnSessionCloseEvent(
  vzsdk::Session::Ptr session,
  int code,
  uint32 connect_id) {
  //
  MessageData::Ptr stanza(new Stanza(RES_DISCONNECTED_EVENT));
  queue_layer_->Post(connect_id, stanza);
  LOG(LS_INFO) << "remote disconnected";
  bool remove_res = RemoveSession(session->session_id());
  ASSERT(remove_res);
}

}
