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

#ifndef SRC_HSHA_SESSION_MANAGER_LAYER_H_
#define SRC_HSHA_SESSION_MANAGER_LAYER_H_

#include <map>

#include "base/noncopyable.h"
#include "event/thread.h"
#include "base/scoped_ptr.h"
#include "vzsdk/session.h"

namespace vzsdk {

class QueueLayer;

class SessionManager : public noncopyable,
  public sigslot::has_slots<>,
  public MessageHandler  {
 public:
  SessionManager(QueueLayer *queue_layer);
  virtual ~SessionManager();
  virtual void OnMessage(Message *msg);
  void Post(uint32 task_id, MessageData::Ptr pdata);
  bool Start();
  bool Stop();
 private:
  bool InitAsyncThread();
  void BindSessionSignal(Session::Ptr session);
  void UnbindSessionSignal(Session::Ptr session);

  bool AddSession(Session::Ptr session);
  bool RemoveSession(uint32 session_id);
  Session::Ptr FindSession(uint32 session_id);

  void OnConnectMessage(Message *msg);
  void OnDisconnectMessage(Message *msg);
  void OnRequestMessage(Message *msg);

  void OnSessionPacketEvent(Session::Ptr session,
                            const char *data,
                            uint32 data_size,
                            uint8 packet_type);
  void OnSessionConnectedEvent(Session::Ptr session,
                               uint32 connect_id);
  void OnSessionCloseEvent(Session::Ptr session,
                           int code,
                           uint32 connect_id);
private:
  const char *FindEndString(const char *data, uint32 data_size);
 private:
  scoped_ptr<Thread> async_thread_;
  std::map<uint32, Session::Ptr> async_sessions_;
  QueueLayer *queue_layer_;
};

}

#endif // SRC_HSHA_SESSION_MANAGER_LAYER_H_