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

#include "vzsdk/vzsdkservices.h"
#include "vzsdk/task.h"
#include "base/logging.h"

namespace vzsdk {

VzsdkServices::VzsdkServices() {
}

VzsdkServices::~VzsdkServices() {
}

bool VzsdkServices::Start() {
  // Init logging system
  LogMessage::LogTimestamps(true);
  LogMessage::LogContext(vzsdk::LoggingSeverity::LS_INFO);
  LogMessage::LogThreads(true);
  queue_layer_.reset(new QueueLayer());
  return queue_layer_->Start();
}

bool VzsdkServices::Stop() {
  return queue_layer_->Stop();
}

int VzsdkServices::ConnectServer(const std::string ip_addr, uint16 port) {
  SocketAddress remote_address(ip_addr, port);
  ConnectTask connect_task(queue_layer_.get(),
                           DEFAULT_TIMEOUT,
                           remote_address);
  Message::Ptr msg = connect_task.SyncProcessTask();
  if(!msg) {
    return DEFAULT_RESULT_TIMEOUT;
  }
  Stanza *stanza = static_cast<Stanza *>(msg->pdata);
  return stanza->session_id();
}

}
