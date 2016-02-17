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

#include "vzsdk/task.h"
#include "vzsdk/queuelayer.h"

namespace vzsdk {

// The task id start by 0X01
uint32 Task::unequal_task_id_ = 0X01;

Task::Task(QueueLayer *queue_layer, uint32 timeout)
  :queue_layer_(queue_layer),
   timeout_(timeout) {
  task_thread_ = Thread::Current();
  task_id_ = unequal_task_id_++;
}

Task::~Task() {
}

void Task::OnMessage(Message *msg) {
  ASSERT(false);
}

Message::Ptr Task::SyncProcessTask() {
  PostTask();
  return WaitTaskDone();
}

void Task::HandleMessage(MessageData::Ptr pdata) {
  task_thread_->Post(this, task_id_, pdata);
}

void Task::PostTask() {
  ASSERT(queue_layer_ != NULL);
  queue_layer_->Post(shared_from_this());
}

Message::Ptr Task::WaitTaskDone() {
  Thread *current_thread = Thread::Current();
  uint32 end_time = Time() + timeout_;
  Message::Ptr msg(new Message());
  while(1) {
    current_thread->Peek(msg.get(), end_time - Time());
    if(msg->phandler == NULL) {
      msg.reset(NULL);
      break;
    } else if(msg->phandler != NULL && msg->message_id == task_id_) {
      break;
    } else {
      current_thread->Dispatch(msg.get());
    }
  }
  return msg;
}

ConnectTask::ConnectTask(QueueLayer *queue_layer,
                         uint32 timeout,
                         SocketAddress &address)
  : Task(queue_layer, timeout),
    message_data_(new ReqConnectData(address, REQ_CONNECT_SERVER)) {
}

ConnectTask::~ConnectTask() {
}
}