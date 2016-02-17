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

#include "vzsdk/queuelayer.h"
#include "vzsdk/internalmessage.h"
#include "vzsdk/task.h"

namespace vzsdk {

class ReqTaskStanza : public Stanza {
 public:
  ReqTaskStanza(Task::Ptr task)
    : Stanza(task->message_type()),
      task_(task) {
  }
  virtual ~ReqTaskStanza() {
  }
  Task::Ptr task() {
    return task_;
  }
 private:
  Task::Ptr task_;
};

QueueLayer::QueueLayer() {
}

QueueLayer::~QueueLayer() {
}

void QueueLayer::OnMessage(Message *msg) {
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  if(stanza->stanza_type() > REQ_RES_BREAK) {
    OnResMessage(msg);
  } else if (stanza->stanza_type() < REQ_RES_BREAK) {
    OnReqMessage(msg);
  } else {
    ASSERT(false);
  }
}

void QueueLayer::OnReqMessage(Message *msg) {
  ReqTaskStanza *stanza_task = static_cast<ReqTaskStanza *>(msg->pdata.get());
  Task::Ptr task = stanza_task->task();
  bool add_res = AddTask(task);
  ASSERT(add_res);
  session_manager_->Post(task->task_id(), task->message_data());
}

void QueueLayer::OnResMessage(Message *msg) {
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  switch (stanza->stanza_type()) {
  case RES_DISCONNECTED_EVENT:
  case RES_CONNECTED_EVENT:
    OnConnectedEvent(msg->message_id, msg);
    break;
  case RES_STANZA_EVENT:
    break;
  default:
    break;
  }
}

void QueueLayer::OnConnectedEvent(uint32 task_id, Message *msg) {
  Task::Ptr task = FindTask(task_id);
  if(task != NULL) {
    task->HandleMessage(msg->pdata);
  }
}

void QueueLayer::Post(uint32 id, MessageData::Ptr pdata) {
  queue_thread_->Post(this, id, pdata);
}

void QueueLayer::Post(Task::Ptr task) {
  MessageData::Ptr stanza_task(new ReqTaskStanza(task));
  queue_thread_->Post(this, task->task_id(), stanza_task);
}

bool QueueLayer::Start() {
  queue_thread_.reset(new Thread());
  session_manager_.reset(new SessionManager(this));
  if(queue_thread_->Start()) {
    return session_manager_->Start();
  }
  return false;
}

bool QueueLayer::Stop() {
  return true;
}


bool QueueLayer::AddTask(Task::Ptr task) {
  std::map<uint32, Task::Ptr>::iterator iter = tasks_.find(task->task_id());
  if(iter != tasks_.end()) {
    return false;
  }
  tasks_.insert(std::pair<uint32, Task::Ptr>(task->task_id(), task));
  return true;
}

bool QueueLayer::RemoveTask(uint32 task_id) {
  std::map<uint32, Task::Ptr>::iterator iter = tasks_.find(task_id);
  if(iter == tasks_.end()) {
    return false;
  }
  tasks_.erase(iter);
  return true;
}

Task::Ptr QueueLayer::FindTask(uint32 task_id) {
  std::map<uint32, Task::Ptr>::iterator iter = tasks_.find(task_id);
  if(iter == tasks_.end()) {
    return Task::Ptr();
  }
  return iter->second;
}

}