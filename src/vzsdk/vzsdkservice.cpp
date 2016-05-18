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

#include "vzsdk/vzsdkservice.h"
#include "vzsdk/task.h"
#include "base/logging.h"
#include "vzsdk/vzsdkbase.h"
#include "vzsdk/commandanalysis.h"
#include "vzsdk/vzconnectdev.h"
#include "vzsdk/vzrecognition.h"
#include "vzsdk/vzmaintendev.h"
#include "vzsdk/vzwlistdev.h"
#include "vzsdk/vzserialdev.h"
#include "vzsdk/vziodev.h"

namespace vzsdk {

/************************************************************************/
// 函数:     VzsdkServices
// 返回值:
// 说明:     默认构造函数，初始化
/************************************************************************/
VzsdkService::VzsdkService()
    : push_manager_task_(NULL)
    , conn_callback_(NULL)
    , user_data_(NULL){
}

VzsdkService::~VzsdkService() {
    Stop();
}

int VzsdkService::GetSessionID() {
    return connect_dev_->GetSessionID();
}

/************************************************************************/
// 函数:     Start
// 返回值:   bool
// 说明:     初始化资源函数
/************************************************************************/
bool VzsdkService::Start() {
    initModule();
    return true;
}

/************************************************************************/
// 函数:     Stop
// 返回值:   bool
// 说明:     停止服务
/************************************************************************/
bool VzsdkService::Stop() {
    if (connect_dev_)
      connect_dev_->DisconnectServer();
    return true;
}

void VzsdkService::SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *pUserData)
{
  conn_callback_ = func;
  user_data_ = pUserData;  
}

void VzsdkService::GetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *user_data)
{
  func = conn_callback_;
  user_data = user_data_;
}

void VzsdkService::SetParam(QueueLayerPtr queue_layer, PushManagerTask* push_manager_task, ThreadPtr push_thread)
{
    queue_layer_ = queue_layer;
    push_manager_task_ = push_manager_task;
    push_thread_ = push_thread;
}

void VzsdkService::initModule() {
    VzConnectDevPtr _conncet_dev_ptr(new VzConnectDev(this));
    connect_dev_ = _conncet_dev_ptr;

    VzRecognitionPtr _recognition_ptr(new VzRecognition(this));
    recongition_ = _recognition_ptr;

    VzMaintenDevPtr _mainten_dev_ptr(new VzMaintenDev(this));
    maintenDev_ = _mainten_dev_ptr;

    VzWlistDevPtr _whist_dev_ptr(new VzWlistDev(this));
    whist_dev_ = _whist_dev_ptr;

    VzSerialDevPtr _serial_dev_ptr(new VzSerialDev(this));
    serial_dev_ = _serial_dev_ptr;

    VzIODevPtr _io_dev_ptr(new VzIODev(this));
    io_dev_ = _io_dev_ptr;
}

/************************************************************************/
// 函数:     AddPushHandle
// 参数:     PushHandle::Ptr handle
// 返回值:   void
// 说明:     消息处理队列
/************************************************************************/
void VzsdkService::AddPushHandle(PushHandle::Ptr handle) {
    if (push_manager_task_) {
        push_manager_task_->AddPushHandle(handle);
    }
}

/************************************************************************/
// 函数:     RemovePushHandle
// 参数:     PushHandle::Ptr handle
// 返回值:   void
// 说明:     移除消息
/************************************************************************/
void VzsdkService::RemovePushHandle(PushHandle::Ptr handle) {
    if (push_manager_task_) {
        push_manager_task_->RemovePushHandle(handle);
    }
}

vzsdk::QueueLayerPtr VzsdkService::GetQueueLayer() {
    return queue_layer_;
}

vzsdk::VzConnectDevPtr VzsdkService::GetConnectDev() {
    return connect_dev_;
}

vzsdk::VzRecognitionPtr VzsdkService::GetRecongintion() {
    return recongition_;
}

vzsdk::VzMaintenDevPtr VzsdkService::GetMaintenDev() {
    return maintenDev_;
}

vzsdk::VzWlistDevPtr VzsdkService::GetWlistDev() {
    return whist_dev_;
}

vzsdk::VzSerialDevPtr VzsdkService::GetSerialDev() {
    return serial_dev_;
}

vzsdk::VzIODevPtr VzsdkService::GetIODev() {
    return io_dev_;
}
}
