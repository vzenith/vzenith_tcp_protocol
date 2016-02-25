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

#include "vzsdkpushmanager.h"
#include "vzsdk/task.h"
#include "base/logging.h"
#include "vzsdk/vzsdkbase.h"

namespace vzsdk {

PushManagerTask::PushManagerTask(QueueLayer *queue_layer, Thread *push_thread)
  : Task(queue_layer, 50000, push_thread),
    is_register_(false) {
}

PushManagerTask::~PushManagerTask() {
}

Message::Ptr PushManagerTask::SyncProcessTask() {
  if(!is_register_) {
    PostTask();
    is_register_ = true;
  }
  task_thread_->Start();
  // Block there
  return Message::Ptr();
}

bool PushManagerTask::HandleMessage(Message *msg) {
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  if(msg->message_id == 0
      && stanza->stanza_type() == RES_STANZA_EVENT) {
    return HandleResponse(msg);
  }
  return false;
}

bool PushManagerTask::HandleResponse(Message *msg) {
  CritScope cs(&crit_);
  ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
  const std::string res_cmd = response->res_json()[JSON_REQ_CMD].asString();
  PushHandleKeys::iterator iter = push_handle_keys_.find(res_cmd);
  if(iter != push_handle_keys_.end()) {
    task_thread_->Post(this, task_id_, msg->pdata);
    return true;
  }
  return false;
}

void PushManagerTask::OnMessage(Message *msg) {
  ProcessPushEvent(msg);
}

void PushManagerTask::ProcessPushEvent(Message *msg) {
  CritScope cs(&crit_);
  ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
  for(std::size_t i = 0; i < push_handles_.size(); i++) {
    if(push_handles_[i]->HandleMessageData(response)) {
      break;
    }
  }
}

void PushManagerTask::AddPushHandle(PushHandle::Ptr handle) {
  CritScope cs(&crit_);
  ASSERT(handle.get() != NULL);
  push_handles_.push_back(handle);
  push_handle_keys_.insert(handle->cmd_key());
}

void PushManagerTask::RemovePushHandle(PushHandle::Ptr handle) {
  CritScope cs(&crit_);
  ASSERT(handle.get() != NULL);
  PushHandleKeys::iterator iter = push_handle_keys_.find(handle->cmd_key());
  if(iter != push_handle_keys_.end()) {
    push_handle_keys_.erase(iter);
  }
  for(std::size_t i = 0; i < push_handles_.size(); i++) {
    if(push_handles_[i] == handle) {
      push_handles_.erase(push_handles_.begin() + i);
      break;
    }
  }
}
}
