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
#include "vzsdk/queuelayer.h"
#include "vzsdk/task.h"

class MessageExample : public vzsdk::MessageHandler {
 public:
  virtual void OnMessage(vzsdk::Message* msg) {
    LOG(LS_INFO) << "OnMessage of Message Example";
  }
 private:
};

int main(void) {
  vzsdk::Thread *main_thread = vzsdk::Thread::Current();
  std::cout << "event/thread.h" << std::endl;
  //
  vzsdk::QueueLayer queue_layer;
  ASSERT(queue_layer.Start());
  vzsdk::SocketAddress remote_address("192.168.4.172", 8131);
  vzsdk::ConnectTask connect_task(&queue_layer, 5000, remote_address);
  vzsdk::Message::Ptr msg = connect_task.SyncProcessTask();
  return EXIT_SUCCESS;
}