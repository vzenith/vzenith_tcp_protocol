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

#include <iostream>
#include "event/thread.h"
#include "base/logging.h"
#include "vzsdk/session.h"

class SessionManager : public vzsdk::MessageHandler,
  public sigslot::has_slots<> {
 public:
  SessionManager(vzsdk::Thread *async_thread)
    : async_thread_(async_thread) {
  }
  void Start() {
    ASSERT(async_thread_ != NULL);
    session_.reset(new vzsdk::Session(async_thread_));
    BindSessionSignal();
    vzsdk::SocketAddress remote_address("192.168.4.172", 8131);
    session_->Start(remote_address);
  }
  virtual void OnMessage(vzsdk::Message* msg) {
    LOG(LS_INFO) << "Disconnect this session";
    if(session_ && session_->GetState() != vzsdk::Socket::CS_CLOSED) {
      LOG(LS_INFO) << "session is value";
      session_->Stop();
      UnbindSessionSignal();
      session_.reset(NULL);
    }
  }
 private:
  void OnSessionPacketEvent(vzsdk::Session::Ptr session,
                            const char *data,
                            uint32 data_size,
                            uint8 packet_type) {
    //
    LOG(LS_INFO).write(data, data_size);
  }

  void OnSessionConnectedEvent(vzsdk::Session::Ptr session, uint32 id) {
    //
    LOG(LS_INFO) << "remote connect succeed";
  }

  void OnSessionCloseEvent(vzsdk::Session::Ptr session, int code, uint32 id) {
    //
    LOG(LS_INFO) << "remote disconnected";
  }
  void BindSessionSignal() {
    session_->SignalSessionCloseEvent.connect(
      this, &SessionManager::OnSessionCloseEvent);
    session_->SignalSessionConnectedEvent.connect(
      this, &SessionManager::OnSessionConnectedEvent);
    session_->SignalSessionPacketEvent.connect(
      this, &SessionManager::OnSessionPacketEvent);
  }
  void UnbindSessionSignal() {
    session_->SignalSessionCloseEvent.disconnect_all();
    session_->SignalSessionConnectedEvent.disconnect_all();
    session_->SignalSessionPacketEvent.disconnect_all();
  }
 private:
  vzsdk::scoped_ptr<vzsdk::Session> session_;
  vzsdk::Thread *async_thread_;
};

int main(void) {
  // Initialize the logging system
  vzsdk::LogMessage::LogTimestamps(true);
  vzsdk::LogMessage::LogContext(vzsdk::LS_INFO);
  vzsdk::LogMessage::LogThreads(true);

  vzsdk::Thread *main_thread = vzsdk::Thread::Current();
  SessionManager sm(main_thread);
  // main_thread->Start();
  main_thread->PostDelayed(10000, &sm);
  sm.Start();
  main_thread->Run();
  return EXIT_SUCCESS;
}