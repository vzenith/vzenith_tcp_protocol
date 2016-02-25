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
#include "vzsdk/vzsdkbase.h"

namespace vzsdk {

VzsdkServices::VzsdkServices()
  : push_manager_task_(NULL) {
}

VzsdkServices::~VzsdkServices() {
}

bool VzsdkServices::Start() {
  // Init logging system
  LogMessage::LogTimestamps(true);
  LogMessage::LogContext(vzsdk::LS_INFO);
  LogMessage::LogThreads(true);
  queue_layer_.reset(new QueueLayer());
  ASSERT(push_manager_task_ == NULL);
  push_thread_.reset(new Thread());
  push_manager_task_ = new PushManagerTask(queue_layer_.get(),
      push_thread_.get());
  // For push manager life live
  Task::Ptr push_task(push_manager_task_);
  if(queue_layer_->Start()) {
    push_manager_task_->SyncProcessTask();
    return true;
  }
  return false;
}

bool VzsdkServices::Stop() {
  return queue_layer_->Stop();
}

void VzsdkServices::AddPushHandle(PushHandle::Ptr handle) {
  if(push_manager_task_) {
    push_manager_task_->AddPushHandle(handle);
  }
}

void VzsdkServices::RemovePushHandle(PushHandle::Ptr handle) {
  if(push_manager_task_) {
    push_manager_task_->RemovePushHandle(handle);
  }
}

int VzsdkServices::ConnectServer(const std::string ip_addr, uint16 port) {
  SocketAddress remote_address(ip_addr, port);
  Task::Ptr connect_task(new ConnectTask(queue_layer_.get(),
                                         FOREVER_TIMEOUT,
                                         remote_address));
  Message::Ptr msg = connect_task->SyncProcessTask();
  if(!msg || msg->phandler == NULL) {
    return DEFAULT_RESULT_TIMEOUT;
  }
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  return stanza->session_id();
}

int VzsdkServices::DisconnectServer(uint32 session_id) {
  if(session_id == 0) {
    LOG(LS_WARNING) << "The session is is zero, is not a right session";
    return SESSION_ID_INVALUE;
  }
  Task::Ptr disconnect_task(new DisconnectTask(queue_layer_.get(),
                            DEFAULT_TIMEOUT,
                            session_id));
  Message::Ptr msg = disconnect_task->SyncProcessTask();
  if(!msg || msg->phandler == NULL) {
    return DEFAULT_RESULT_TIMEOUT;
  }
  Stanza *stanza = static_cast<Stanza *>(msg->pdata.get());
  if(stanza->stanza_type() == RES_DISCONNECTED_EVENT_SUCCEED) {
    LOG(LS_INFO) << "Disconnect succeed";
    return REQ_SUCCEED;
  } else if (stanza->stanza_type() == RES_DISCONNECTED_EVENT_FAILURE) {
    return SESSION_NOT_FOUND;
  } else {
    return UNKOWN_ERROR;
  }
  return REQ_SUCCEED;
}

int VzsdkServices::GetDeviceSN(uint32 session_id, std::string *sn) {
  if(session_id == 0) {
    LOG(LS_WARNING) << "The session is is zero, is not a right session";
    return SESSION_ID_INVALUE;
  }

  Json::Value req_json;
  req_json[JSON_REQ_CMD] = JSON_REQ_CMD_GETSN;

  Task::Ptr req_task(new ReqTask(queue_layer_.get(),
                                 DEFAULT_TIMEOUT,
                                 session_id,
                                 req_json));
  Message::Ptr msg = req_task->SyncProcessTask();
  if(!msg || msg->phandler == NULL) {
    return DEFAULT_RESULT_TIMEOUT;
  }
  ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
  *sn = response->res_json()[JSON_VALUE].asString();
  return REQ_SUCCEED;
}

int VzsdkServices::ReciveIvsResult(uint32 session_id,
                                   PushHandle::Ptr handle,
                                   bool enable_result,
                                   IvsFormat format,
                                   bool enable_img,
                                   IvsImgType img_type) {
  if(session_id == 0) {
    LOG(LS_WARNING) << "The session is is zero, is not a right session";
    return SESSION_ID_INVALUE;
  }

  Json::Value req_json;
  req_json[JSON_REQ_CMD] = JSON_PUSH_CMD_IVSRESULT;
  req_json[JSON_REQ_IVSRESULT_ENABLE] = enable_result;
  req_json[JSON_REQ_IVSRESULT_FORMAT] = IvsFormatToString(format);
  req_json[JSON_REQ_IVSRESULT_ENABLE_IMAGE] = enable_img;
  req_json[JSON_REQ_IVSRESULT_IMAGE_TYPE] = (int)(img_type);
  AddPushHandle(handle);
  Task::Ptr req_task(new ReqPushTask(queue_layer_.get(),
                                     DEFAULT_TIMEOUT,
                                     session_id,
                                     req_json));
  Message::Ptr msg = req_task->SyncProcessTask();
  if(!msg || msg->phandler == NULL) {
    return DEFAULT_RESULT_TIMEOUT;
  }
  ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
  //LOG(LS_INFO) << response->res_data();
  return REQ_SUCCEED;
}

std::string VzsdkServices::IvsFormatToString(IvsFormat ivs_format) {
  switch (ivs_format) {
  case vzsdk::VzsdkServices::FORMAT_BIN:
    return "bin";
    break;
  case vzsdk::VzsdkServices::FORMAT_JSON:
    return "json";
    break;
  default:
    break;
  }
  return "bin";
}

}
