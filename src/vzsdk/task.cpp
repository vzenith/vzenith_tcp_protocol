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
#include "base/logging.h"
#include "vzsdk/vzsdkbase.h"

#ifndef __has_feature
#define __has_feature(x) 0  // Compatibility with non-clang or LLVM compilers.
#endif  // __has_feature

namespace vzsdk {

// The task id start by 0X01
uint32 Task::unequal_task_id_ = 0X01;

Task::Task(QueueLayer *queue_layer, uint32 timeout, Thread *task_thread)
    :queue_layer_(queue_layer),
     timeout_(timeout),
     task_thread_(task_thread) {
    if(task_thread_ == NULL) {
        task_thread_ = Thread::Current();
    }
    task_id_ = unequal_task_id_++;
    // LOG(LS_INFO) << "Create task " << task_id_;
}

Task::~Task() {
    // LOG(LS_INFO) << "Destory task " << task_id_;
}

void Task::OnMessage(Message *msg) {
    ASSERT(false);
}

Message::Ptr Task::SyncProcessTask() {
    PostTask();
    Message::Ptr msg = WaitTaskDone();
    queue_layer_->AsyncRemoveTask(shared_from_this());
    return msg;
}

bool Task::HandleMessage(Message *msg) {
    if(msg->message_id == task_id_) {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

void Task::PostTask() {
    ASSERT(queue_layer_ != NULL);
    queue_layer_->Post(shared_from_this());
}

Message::Ptr Task::WaitTaskDone() {
    uint32 msEnd = TimeAfter(timeout_);
    int cmsNext = timeout_;
    Message::Ptr msg(new Message());
    while(1) {
#if __has_feature(objc_arc)
        @autoreleasepool
#elif defined(OSX) || defined(IOS)
        // see: http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/Foundation/Classes/NSAutoreleasePool_Class/Reference/Reference.html
        // Each thread is supposed to have an autorelease pool. Also for event loops
        // like this, autorelease pool needs to be created and drained/released
        // for each cycle.
        ScopedAutoreleasePool pool;
#endif
        task_thread_->Get(msg.get(), cmsNext);
        if(msg->phandler == NULL) {
            // msg.reset(NULL);
        } else if(msg->phandler != NULL && msg->message_id == task_id_) {
            break;
        } else {
            // Unhandle Error
            LOG(LS_ERROR) << "Unhandle Message";
            ASSERT(false);
            // current_thread->Dispatch(msg.get());
        }
        cmsNext = TimeUntil(msEnd);
        if (cmsNext < 0)
            break;
    }
    return msg;
}

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
ConnectTask::ConnectTask(QueueLayer *queue_layer,
                         uint32 timeout,
                         SocketAddress &address)
    : Task(queue_layer, timeout),
      req_connect_data_(new ReqConnectData(address)) {
    set_message_data(req_connect_data_);
}

ConnectTask::ConnectTask(QueueLayer *queue_layer
                         , uint32 timeout
                         , const ReqConnectData& _req_connect_data)
    : Task(queue_layer, timeout)
    , req_connect_data_(new ReqConnectData(_req_connect_data)) {

}

ConnectTask::~ConnectTask() {
}

//------------------------------------------------------------------------------
DisconnectTask::DisconnectTask(QueueLayer *queue_layer,
                               uint32 timeout,
                               uint32 session_id)
    : Task(queue_layer, timeout),
      req_disconnect_data_(new ReqDisconnectData(session_id)) {
    set_message_data(req_disconnect_data_);
}

bool DisconnectTask::HandleMessage(Message *msg) {
    Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
    if(msg->message_id == task_id_
            && (stanza->stanza_type() == RES_DISCONNECTED_EVENT_SUCCEED
                || stanza->stanza_type() == RES_DISCONNECTED_EVENT_FAILURE)
            && stanza->session_id() == req_disconnect_data_->session_id()) {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

DisconnectTask::~DisconnectTask() {
}

//------------------------------------------------------------------------------
ReqTask::ReqTask(QueueLayer *queue_layer,
                 uint32 timeout,
                 uint32 session_id,
                 const Json::Value &req_json)
    : Task(queue_layer, timeout),
      req_data_(new RequestData(session_id, req_json)) {
    set_message_data(req_data_);
    req_cmd_ = req_json[JSON_REQ_CMD].asString();
    ASSERT(!req_cmd_.empty());
}
ReqTask::~ReqTask() {
}

bool ReqTask::HandleMessage(Message *msg) {
    Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
    if(msg->message_id == 0
            && stanza->session_id() == req_data_->session_id()
            && stanza->stanza_type() == RES_STANZA_EVENT) {
        return HandleResponse(msg);
    }
    if(msg->message_id == task_id_
            && stanza->session_id() == req_data_->session_id()) {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

bool ReqTask::HandleResponse(Message *msg) {
    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    const std::string res_cmd = response->res_json()[JSON_REQ_CMD].asString();
    if(res_cmd == req_cmd_) {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

ReqPushTask::ReqPushTask(QueueLayer *queue_layer,
                         uint32 timeout,
                         uint32 session_id,
                         const Json::Value &req_json)
    : Task(queue_layer, timeout),
      req_data_(new RequestData(session_id, req_json, true)) {
    set_message_data(req_data_);
}
ReqPushTask::~ReqPushTask() {
}

bool ReqPushTask::HandleMessage(Message *msg) {
    Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
    if(msg->message_id == task_id_
            && stanza->session_id() == req_data_->session_id()) {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

ReConnectTask::ReConnectTask(QueueLayer *queue_layer
                             , uint32 timeout
                             , const ReqConnectData& _req_connect_data)
    : Task(queue_layer, timeout)
    , req_reconnect_data_(new ReqConnectData(_req_connect_data)) {
    set_message_data(req_reconnect_data_);
}

ReConnectTask::~ReConnectTask() {
}

void ReConnectTask::OnMessage(Message *msg) {

}

ReqRecordTask::ReqRecordTask(QueueLayer *queue_layer
                             , uint32 timeout
                             , uint32 session_id
                             , const Json::Value &req_json)
    : ReqTask(queue_layer, timeout, session_id, req_json) {

}

ReqRecordTask::~ReqRecordTask() {

}

bool ReqRecordTask::HandleResponse(Message *msg) {
    //»ñÈ¡¼ÇÂ¼
    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    const std::string res_cmd = response->res_json()[JSON_REQ_CMD].asString();
    if (res_cmd == "ivs_result" &&  req_cmd_ == "get_record") {
        task_thread_->Post(this, task_id_, msg->pdata);
        return true;
    }
    return false;
}

}