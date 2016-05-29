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
#include "vzsdk/vzconnectdev.h"
#include "vzsdk/vzsdkdefines.h"
#include "base/logging.h"
#include "vzsdk/vzsdkpushhandle.h"


vzsdk::VzConnectDev::VzConnectDev(VzsdkService* _service)
    : VZModuleBase(_service) {
}

vzsdk::VzConnectDev::~VzConnectDev() {
    DisconnectServer();
}

/************************************************************************/
// ����:     ConnectServer
// ����:     const std::string & _ip_addr
// ����:     uint16 _port
// ����ֵ:   int
// ˵��:     ���ӵ��豸
/************************************************************************/
int vzsdk::VzConnectDev::ConnectServer(const std::string& _ip_addr, uint16 _port) {
    int _session_id = SESSION_ID_INVALUE;
    SocketAddress remote_address(_ip_addr, _port);
    Task::Ptr connect_task(new ConnectTask(sdk_service_->GetQueueLayer().get(),
										   DEFAULT_TIMEOUT,
                                           remote_address));
    Message::Ptr msg = connect_task->SyncProcessTask();
    ReqConnectData* _req_connect_data = static_cast<ReqConnectData*>(connect_task->message_data().get());
    req_connect_data_ptr_ = ReqConnectDataPtr(new ReqConnectData(*_req_connect_data));

    if (!msg || msg->phandler == NULL) {        
        return DEFAULT_RESULT_TIMEOUT;
    }
  

    Stanza* _stanze = static_cast<Stanza*>(msg->pdata.get());
    if (_stanze) {
        int _new_session_id = _stanze->session_id();
        req_connect_data_ptr_->set_session_id(_new_session_id);
        if (GetConnState() == Socket::CS_CONNECTED) {
            _session_id = _new_session_id;
            ReConnectServer();
            ChangeConn();
        }
        else 
            req_connect_data_ptr_->set_session_id(SESSION_ID_INVALUE);
    }

    return _session_id;
}

/************************************************************************/
// ����:     DisconnectServer
// ����:     uint32 session_id
// ����ֵ:   int
// ˵��:     �ر�����
/************************************************************************/
int vzsdk::VzConnectDev::DisconnectServer() {
    int _session_id = req_connect_data_ptr_->session_id();
    if (_session_id == SESSION_ID_INVALUE) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return SESSION_ID_INVALUE;
    }
    SignalDeviceDisconnecing.emit();
    Task::Ptr disconnect_task(new DisconnectTask(sdk_service_->GetQueueLayer().get(),
                              DEFAULT_TIMEOUT,
                              _session_id));
    Message::Ptr msg = disconnect_task->SyncProcessTask();
    if (!msg || msg->phandler == NULL) {
        return DEFAULT_RESULT_TIMEOUT;
    }
    Stanza *stanza = static_cast<Stanza*>(msg->pdata.get());
    if (stanza && stanza->stanza_type() == RES_DISCONNECTED_EVENT_SUCCEED) {
        LOG(LS_INFO) << "Disconnect succeed";
        return REQ_SUCCEED;
    } else if (stanza->stanza_type() == RES_DISCONNECTED_EVENT_FAILURE) {
        return SESSION_NOT_FOUND;
    } else {
        return UNKOWN_ERROR;
    }
    return REQ_SUCCEED;
}

/************************************************************************/
// ����:     GetIP
// ����ֵ:   std::string
// ˵��:     ��ȡ�����豸��IP
/************************************************************************/
std::string vzsdk::VzConnectDev::GetIP() {
    return req_connect_data_ptr_->address().ipaddr().ToString();
}

/************************************************************************/
// ����:     GetSessionID
// ����ֵ:   int
// ˵��:     ��ȡsession id
/************************************************************************/
int vzsdk::VzConnectDev::GetSessionID() {
    int session_id = SESSION_ID_INVALUE;
    if (req_connect_data_ptr_)
        session_id = req_connect_data_ptr_->session_id();
    return session_id;
}

/************************************************************************/
// ����:     GetConnState
// ����ֵ:   vzsdk::Socket::ConnState
// ˵��:     ��ǰ����״̬
/************************************************************************/
vzsdk::Socket::ConnState vzsdk::VzConnectDev::GetConnState() {
    int _session_id = GetSessionID();
    SessionManager::Ptr session_mana_ = sdk_service_->GetQueueLayer()->GetSessionManamger();
    vzsdk::Session::Ptr _session_ptr = session_mana_->FindSession(_session_id);
    if (_session_ptr) {
        return _session_ptr->GetState();
    }
    return Socket::CS_CLOSED;
}

/************************************************************************/
// ����:     SetCommonNotifyCallBack
// ����:     VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func
// ����:     void * pUserData
// ����ֵ:   void
// ˵��:     ���ö��������ص�����
/************************************************************************/
void vzsdk::VzConnectDev::SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *pUserData) {
    static_cast<ChangeConnPushHandle*>(change_conn_pushhandle.get())->SetConnCallBack(func, pUserData);
}

/************************************************************************/
// ����:     ReConnectServer
// ����ֵ:   int
// ˵��:     ��������
/************************************************************************/
int vzsdk::VzConnectDev::ReConnectServer() {
    if (!req_connect_data_ptr_)
        return SESSION_ID_INVALUE;

    req_connect_data_ptr_->set_stanza_type(RES_RECONNECT_EVENT);
    sdk_service_->GetQueueLayer()->Post(RES_RECONNECT_EVENT, req_connect_data_ptr_);

    return REQ_SUCCEED;
}

/************************************************************************/
// ����:     ChangeConn
// ����ֵ:   void
// ˵��:     ��������״̬�ı�
/************************************************************************/
void vzsdk::VzConnectDev::ChangeConn() {
    change_conn_pushhandle = ChangeConnPushHandle::Ptr(new ChangeConnPushHandle("change_conn_status"));    
    ChangeConnPushHandle* conn_handle = static_cast<ChangeConnPushHandle*>(change_conn_pushhandle.get());
    conn_handle->SignalChangeConnStatus.connect(this, &VzConnectDev::SlotChangeConnStatus);
    conn_handle->SetSessionID(sdk_service_->GetSessionID());

    VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func = NULL;
    void *user_data = NULL;
    sdk_service_->GetCommonNotifyCallBack(func, user_data);
    conn_handle->SetConnCallBack(func, user_data);
    sdk_service_->AddPushHandle(change_conn_pushhandle);
}

/************************************************************************/
// ����:     SlotChangeConnStatus
// ����:     int statue
// ����ֵ:   void
// ˵��:     ��������״̬�ź�
/************************************************************************/
void vzsdk::VzConnectDev::SlotChangeConnStatus(int statue)
{
    SignalDeviceChangeConnStatus.emit(statue);
}
