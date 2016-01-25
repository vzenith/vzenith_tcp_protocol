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

#ifndef SRC_HSHA_TASK_H_
#define SRC_HSHA_TASK_H_

#include "base/noncopyable.h"
#include "event/thread.h"
#include "base/scoped_ptr.h"
#include "vzsdk/internalmessage.h"

namespace vzsdk {

class QueueLayer;

class Task : public MessageHandler  {
 public:
  Task(QueueLayer *queue_layer, uint32 timeout);
  virtual ~Task();
  virtual void OnMessage(Message *msg);
  virtual MessageData *message_data() = 0;
  virtual uint32 message_type() = 0;
  Message::Ptr SyncProcessTask();
  Thread *task_thread() {
    return task_thread_;
  }
  uint32 task_id() const {
    return task_id_;
  }
  void HandleMessage(MessageData *pdata);
 private:
  void PostTask();
  Message::Ptr WaitTaskDone();
 private:
  static uint32 unequal_task_id_;
  uint32 task_id_;
  QueueLayer *queue_layer_;
  uint32 timeout_;
  Thread *task_thread_;
};

class ConnectTask : public Task {
 public:
  ConnectTask(QueueLayer *queue_layer,
              uint32 timeout,
              SocketAddress &address);
  virtual ~ConnectTask();
  virtual MessageData *message_data() {
    return message_data_.get();
  }
  virtual uint32 message_type() {
    return REQ_CONNECT_SERVER;
  }
 private:
  scoped_ptr<ReqConnectData> message_data_;
};

}

#endif // SRC_HSHA_TASK_H_