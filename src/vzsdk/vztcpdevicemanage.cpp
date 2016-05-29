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
#include "vzsdk/vztcpdevicemanage.h"
#include "vzsdk/vzconnectdev.h"
#include "vzsdkservice.h"
#include "vzsdk/vzrecognition.h"

const int _c_success = 0;
const int _c_failed = -1;

vzsdk::VzTcpDeviceManage::VzTcpDeviceManage() 
            : conn_callback_(NULL)
            , user_data_(NULL)
            , push_manager_task_(NULL){
}

/************************************************************************/
// 函数:     ~VzTcpDeviceManage
// 返回值:
// 说明:     析构，释放资源
/************************************************************************/
vzsdk::VzTcpDeviceManage::~VzTcpDeviceManage() {
    Stop();
}

/************************************************************************/
// 函数:     ExistService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     是否存在session_id对应的服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::ExistService(int session_id) {
    CritScope crit_scope(&crit_section_);
    bool bExist = true;
    VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
    if (it == vzsdk_service_map_.end())
        bExist = false;
    return bExist;
}

/************************************************************************/
// 函数:     GetService
// 参数:     int _session_id
// 返回值:   const VzsdkServicesPtr
// 说明:     根据session_id获取到服务
/************************************************************************/
const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(int session_id) {
    CritScope crit_scope(&crit_section_);
    VzsdkServicesPtr vzsdk_service;
    if (ExistService(session_id)) {
        VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
        vzsdk_service = it->second;
    }
    return vzsdk_service;
}

const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(const std::string& ip) {
    CritScope crit_scope(&crit_section_);
    VzsdkServicesPtr vzsdk_service;
    for (VzsdkServicesMap::iterator it = vzsdk_service_map_.begin();
            it != vzsdk_service_map_.end();
            ++it) {
        VzConnectDevPtr _connect_ptr = it->second->GetConnectDev();
        if (ip == _connect_ptr->GetIP()) {
            vzsdk_service = it->second;
            break;
        }
    }

    return vzsdk_service;
}

/************************************************************************/
// 函数:     RemoveService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     从映射表中移除服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::RemoveService(int session_id) {
    CritScope crit_scope(&crit_section_);
    bool bRet = false;
    VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
    if (it != vzsdk_service_map_.end()) {
        vzsdk_service_map_.erase(it);
        bRet = true;
    }

    return bRet;
}

/************************************************************************/
// 函数:     CreateNewService
// 参数:     const std::string & _ip
// 参数:     const int _port
// 参数:     const std::string & _user_name
// 参数:     const std::string & _user_pwd
// 返回值:   int
// 说明:     创建新的服务
/************************************************************************/
int vzsdk::VzTcpDeviceManage::CreateNewService(const std::string& ip, const int port, const std::string& user_name, const std::string& user_pwd) {
    VzsdkServicesPtr sdk_service(new VzsdkService);
    sdk_service->Start();
    sdk_service->SetParam(queue_layer_, push_manager_task_, push_thread_);
    int session_id = sdk_service->GetConnectDev()->ConnectServer(ip, port);
    if (session_id != SESSION_ID_INVALUE) {
        sdk_service->SetCommonNotifyCallBack(conn_callback_, user_data_);;
        vzsdk_service_map_.insert(std::make_pair(session_id, sdk_service));
    }
    else
        sdk_service->Stop();
    return session_id;
}

/************************************************************************/
// 函数:     CloseService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     关闭服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::CloseService(int session_id) {
    CritScope crit_scope(&crit_section_);
    if (!ExistService(session_id))
        return false;

    GetService(session_id)->Stop();
    RemoveService(session_id);
    return true;
}

void vzsdk::VzTcpDeviceManage::SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func
        , void *user_Data_) {
    conn_callback_ = func;
    user_data_ = user_Data_;
    for (VzsdkServicesMap::iterator it = vzsdk_service_map_.begin();
            it != vzsdk_service_map_.end();
            ++it) {
        it->second->SetCommonNotifyCallBack(func, user_Data_);
        it->second->GetConnectDev()->SetCommonNotifyCallBack(func, user_Data_);
    }
}

void vzsdk::VzTcpDeviceManage::GetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *user_data) {
    func = conn_callback_;
    user_data = user_data_;
}

bool vzsdk::VzTcpDeviceManage::Start()
{
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
    if (queue_layer_->Start()) {
        push_manager_task_->SyncProcessTask();
        return true;
    }
    return false;
}

bool vzsdk::VzTcpDeviceManage::Stop()
{
    vzsdk_service_map_.clear();
    push_thread_->Stop();
    queue_layer_->Stop();
    return true;
}
