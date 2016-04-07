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
#include "base/logging.h"
#include "vzsdkdefines.h"

namespace vzsdk {

class ReqTaskStanza : public Stanza {
 public:
  ReqTaskStanza(Task::Ptr task)
    : Stanza(task->message_type()),
      task_(task) {
  }
  virtual ~ReqTaskStanza() {
    // LOG(LS_INFO) << "Destory ReqTaskStanza ... ...";
  }
  Task::Ptr task() {
    return task_;
  }
 private:
  Task::Ptr task_;
};

class RemoveTaskStanza : public Stanza {
 public:
  RemoveTaskStanza(Task::Ptr task)
    : Stanza(REQ_REMOVE_TASK_EVENT),
      task_(task) {
  }
  virtual ~RemoveTaskStanza() {
    // LOG(LS_INFO) << "Destory ReqTaskStanza ... ...";
  }
  Task::Ptr task() {
    return task_;
  }
 private:
  Task::Ptr task_;
};

////////////////////////////////////////////////////////////////////////////////

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
  Stanza *stanza_task = static_cast<Stanza *>(msg->pdata.get());
  switch (stanza_task->stanza_type()) {
  case REQ_CONNECT_SERVER:
  case REQ_SEND_REQUESTION:
  case REQ_DISCONNECT_SERVER:
    OnTranslateToAsyncLayer(msg);
    break;
  case REQ_REMOVE_TASK_EVENT:
    OnRemoveTaskMessage(msg);
    break;
  case REQ_ADD_PUSH_TASK:
    OnAddPushTaskMessage(msg);
    break;
  default:
    break;
  }
}

void QueueLayer::OnResMessage(Message *msg) {
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  switch (stanza->stanza_type()) {
  case RES_DISCONNECTED_EVENT_SUCCEED:
  case RES_DISCONNECTED_EVENT_FAILURE:
  case RES_CONNECTED_EVENT:
  case RES_STANZA_EVENT:
  case RES_PUSH_SUCCEED:
    OntranslateToSyncLayer(msg);
    break;
  case RES_RECONNECT_SERVER:
    ReConnect(msg);
    break;
  default:
    break;
  }
}

void QueueLayer::OnRemoveTaskMessage(Message *msg) {
  RemoveTaskStanza *stanza_task =
    static_cast<RemoveTaskStanza *>(msg->pdata.get());
  Task::Ptr task = stanza_task->task();
  RemoveTask(task->task_id());
}

void QueueLayer::OnTranslateToAsyncLayer(Message *msg) {
  ReqTaskStanza *stanza_task = static_cast<ReqTaskStanza *>(msg->pdata.get());
  Task::Ptr task = stanza_task->task();
  bool add_res = AddTask(task);
  ASSERT(add_res);
  session_manager_->Post(task->task_id(), task->message_data());
}

void QueueLayer::OnAddPushTaskMessage(Message *msg) {
  ReqTaskStanza *stanza_task = static_cast<ReqTaskStanza *>(msg->pdata.get());
  push_task_ = stanza_task->task();
}

void QueueLayer::OntranslateToSyncLayer(Message *msg) {
  for(std::map<uint32, Task::Ptr>::iterator iter = tasks_.begin();
      iter != tasks_.end(); iter++) {
    if(iter->second->HandleMessage(msg)) {
      return;
    }
  }
  if(push_task_) {
    push_task_->HandleMessage(msg);
  }
}

void QueueLayer::Post(uint32 id, MessageData::Ptr pdata) {
  queue_thread_->Post(this, id, pdata);
}

void QueueLayer::Post(Task::Ptr task) {
  MessageData::Ptr stanza_task(new ReqTaskStanza(task));
  queue_thread_->Post(this, task->task_id(), stanza_task);
}

void QueueLayer::AsyncRemoveTask(Task::Ptr task) {
  MessageData::Ptr stanza_task(new RemoveTaskStanza(task));
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

void QueueLayer::ReConnect(Message* msg)
{
  ReqConnectData* _stanza = static_cast<ReqConnectData*>(msg->pdata.get());
  
  if (_stanza)
  {
    vzsdk::Session::Ptr _session_ptr = session_manager_->FindSession(_stanza->session_id());
    if (!_session_ptr || (_session_ptr && _session_ptr->GetState() == Socket::CS_CLOSED))
    {      
      if (_session_ptr)
        _session_ptr->Stop();
      queue_thread_->Post(session_manager_.get(), REQ_CONNECT_SERVER, msg->pdata);
      //Post(REQ_CONNECT_SERVER, msg->pdata);
    }
    
//     Task::Ptr connect_task(new ReConnectTask(this,
//       vzsdk::FOREVER_TIMEOUT,
//       *_stanza));
    //Message::Ptr msg = connect_task->SyncProcessTask();
    PostDelayed(1000, this, RES_RECONNECT_SERVER, msg->pdata);
 }
}

void QueueLayer::PostDelayed(int cmsDelay, MessageHandler *phandler, uint32 id /*= 0*/, MessageData::Ptr pdata /*= MessageData::Ptr()*/)
{
  queue_thread_->PostDelayed(cmsDelay, phandler, id, pdata);
}

}