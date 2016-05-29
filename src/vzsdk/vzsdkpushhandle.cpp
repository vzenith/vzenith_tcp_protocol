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

#include "vzsdk/vzsdkpushmanager.h"
#include "vzsdk/task.h"
#include "base/logging.h"
#include "vzsdk/vzsdkbase.h"
#include "base/base64.h"
#include "vzsdk/commandanalysis.h"
#include "vzsdk/commfunc.h"
#include "vzsdk/vzsdkpushhandle.h"
#include "vzsdk/vzrecognition.h"

namespace vzsdk {
PushHandle::PushHandle(const std::string &cmd_key)
    :cmd_key_(cmd_key) {
}

PushHandle::~PushHandle() {
}

//------------------------------------------------------------------------------
IvsPushHandle::IvsPushHandle(const std::string &cmd_key)
    : PushHandle(cmd_key)
    , recongnition_(NULL)
    , result_callback_(NULL)
    , result_userdata_(NULL)
    , session_id_(SESSION_ID_INVALUE){
}

IvsPushHandle::~IvsPushHandle() {
}

bool IvsPushHandle::HandleMessageData(ResponseData *response) {
	const Json::Value& res_json = response->res_obj_json();
   
    if (res_json.isNull() || !res_json.isObject() 
        || res_json[JSON_REQ_CMD].isNull())
        return false;

    std::string res_cmd = res_json[JSON_REQ_CMD].asString();
    if (!(response->session_id() == this->session_id_
        && response->stanza_type() == RES_STANZA_EVENT
        && res_cmd == cmd_key()))
        return false;

    LOG(LS_INFO) << response->res_json().toStyledString();

    Json::Value _value = response->res_json();
    TH_PlateResult plate_result_;
    int nFullImgSize = 0, nClipImgSize = 0;
    commandanalysis::ParsePlateResultResponse(_value, plate_result_, nFullImgSize, nClipImgSize);
    //加入已收取到的车牌ID记录
    if (recongnition_)
        recongnition_->addRecordID(plate_result_.uId);

    unsigned char *pImage = NULL;
    unsigned char *pClipImage = NULL;

    int len = strlen(response->res_data().c_str());
    if (nFullImgSize > 0)
        pImage = (unsigned char *)(response->res_data().c_str() + len + 1);
    if (nClipImgSize > 0)
        pClipImage = (unsigned char *)(response->res_data().c_str() + len + nFullImgSize + 1);

    if (result_callback_) {
        result_callback_(session_id_, result_userdata_
                         , &plate_result_, 1
                         , vzsdk::GetResultTypeFromTrigBits(plate_result_.uBitsTrigType)
                         , pImage, nFullImgSize
                         , pClipImage, nClipImgSize);
    }
    return true;
}

void IvsPushHandle::SetPlateCallBack(VZLPRC_TCP_PLATE_INFO_CALLBACK result_callback
                                     , void* result_userdata) {
    result_callback_ = result_callback;
    result_userdata_ = result_userdata;
}

void IvsPushHandle::SetSessionID(int session_id) {
    session_id_ = session_id;
}

void IvsPushHandle::SetRecongnition(VzRecognition* reconition)
{
    recongnition_ = reconition;
}

//------------------------------------------------------------------------------
SerialPushHandle::SerialPushHandle(const std::string &cmd_key)
    : PushHandle(cmd_key) {
    func_		= NULL;
    user_data_  = NULL;
}

SerialPushHandle::~SerialPushHandle() {
}

bool SerialPushHandle::HandleMessageData(ResponseData *response) {
	const Json::Value& res_json = response->res_obj_json();
	if (res_json.isNull() || !res_json.isObject()
		|| res_json[JSON_REQ_CMD].isNull())
		return false;
	std::string res_cmd = response->res_json()[JSON_REQ_CMD].asString();

	if (!(response->stanza_type() == RES_STANZA_EVENT
		&& res_cmd == cmd_key()))
		return false;

    LOG(LS_INFO) << response->res_json().toStyledString();

    Json::Value res_value = response->res_json();

    TTRANSMISSION_RESPONSE value = { 0 };
    commandanalysis::ParseTTransmissionResponse(res_value, &value);

    if (strcmp(value.subcmd, "send") == 0) {
        int comm = value.comm;
        const char *pdata = value.data;

        vector<char> decode_result;
        size_t size_used = 0;
		vzsdk::Base64::DecodeFromArray(pdata, strlen(pdata), Base64::DO_LAX, &decode_result, &size_used);
        if (func_) {
            unsigned int count = decode_result.size();
            unsigned char *buffer = new  unsigned char[count];
            for (int i = 0; i < count; i++) {
                buffer[i] = decode_result[i];
            }

            func_(comm, buffer, count, user_data_);

            delete[] buffer;
        }
    }


    return true;
}

void SerialPushHandle::SetSerialRecvCallBack(VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func
        , void *user_data) {
    func_ = func;
    user_data_ = user_data;
}

ChangeConnPushHandle::ChangeConnPushHandle(const std::string & cmd_key)
    : PushHandle(cmd_key) {

}

ChangeConnPushHandle::~ChangeConnPushHandle() {

}

bool ChangeConnPushHandle::HandleMessageData(ResponseData *response) {
    int stanza_type = response->stanza_type();
    VZ_LPRC_COMMON_NOTIFY notify = VZ_LPRC_NO_ERR;
    std::string detail = "";
    if (response->session_id() == session_id_) {
        if (stanza_type == RES_CONNECTED_EVENT){
            notify = VZ_LPRC_ONLINE;            
        }
        else if (stanza_type == RES_DISCONNECTED_EVENT_FAILURE) {
            notify = VZ_LPRC_OFFLINE;
        }

        if (notify != VZ_LPRC_NO_ERR)
            SignalChangeConnStatus.emit(notify);

        if (notify != VZ_LPRC_NO_ERR && conn_callback_) {            
            conn_callback_(session_id_, user_data_, notify, detail.c_str());
        }
    }
    return false;
}

void ChangeConnPushHandle::SetConnCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void* user_data) {
    conn_callback_ = func;
    user_data_ = user_data;
}

void ChangeConnPushHandle::SetSessionID(int session_id) {
    session_id_ = session_id;
}

ResumePushHandle::ResumePushHandle(const std::string& cmd_key)
    :PushHandle(cmd_key)
    , enable_getrecord_(true)
    , plate_func_(NULL)
    , user_data_(NULL)
    , enable_image_(false)
    , cur_max_record_id_(0)
    , queue_layer_(NULL){

}

ResumePushHandle::~ResumePushHandle(){

}

bool ResumePushHandle::HandleMessageData(ResponseData *response)
{
    int stanza_type = response->stanza_type();
    if (response->stanza_type() == RES_DISCONNECTED_EVENT_SUCCEED
        || response->stanza_type() == RES_DISCONNECTED_EVENT_FAILURE){
        enable_getrecord_ = false;
    }
    if (response->session_id() == session_id_
        && response->stanza_type() == RES_RESUME_TASK_EVENT) {
        bool ret = HandleResponse(response);
                    return ret;
    }
    return false;
}

void ResumePushHandle::AddRecordID(int record_id)
{
    CritScope crit_scope(&cirtical_section_);
    recv_record_id_set_.insert(record_id);
}

void ResumePushHandle::SetSessionID(int session_id)
{
    session_id_ = session_id;
}

void ResumePushHandle::SetStartMaxRecordID(int max_record_id)
{
    max_record_id_ = max_record_id;
}

void ResumePushHandle::SetCallBack(VZLPRC_TCP_PLATE_INFO_CALLBACK func, void* user_data)
{
    plate_func_ = func;
    user_data_ = user_data;
}

bool ResumePushHandle::HandleResponse(ResponseData *response){
    CritScope crit_scope(&cirtical_section_);
    if (RES_RESUME_TASK_EVENT != response->stanza_type() 
        || response->session_id() != session_id_)
        return false;
    
    SignalGetResumeInfo.emit();
    if (plate_func_ == NULL || !enable_getrecord_)
    {
        PostResumeReq();
        return true;
    }
    
    
    if (cur_max_record_id_ > 0){
        for (int id = max_record_id_ + 1; id <= cur_max_record_id_; ++id){
            std::set<int>::iterator it = recv_record_id_set_.find(id);
            if (it != recv_record_id_set_.end())
            {
                max_record_id_ = id;
                recv_record_id_set_.erase(it);
                continue;
            }

            //获取车牌记录
            TH_PlateResult plate_result;
            int full_size, clip_size;
            uchar* fullimage = new uchar[1024 * 768];
            uchar* clipimage = new uchar[1024 * 768];
            recognition_->GetRecord(id, enable_image_, plate_result
                , full_size, fullimage
                , clip_size, clipimage);

            uchar* full_image_param = full_size > 0 ? fullimage : NULL;
            uchar* clip_image_param = clip_size > 0 ? clipimage : NULL;

            //记录回调
            plate_func_(session_id_, user_data_, &plate_result, 1
                , vzsdk::GetResultTypeFromTrigBits(plate_result.uBitsTrigType)
                , full_image_param, full_size
                , clip_image_param, clip_size);
            delete[] fullimage;
            delete[] clipimage;
            max_record_id_ = id;
        }
    }
    PostResumeReq();
    return true;
}

void ResumePushHandle::SlotChangeConnStatus(int status)
{
    switch (status)
    {
    case VZ_LPRC_ONLINE:
    {
        enable_getrecord_ = true;     
        break;
    }
    case VZ_LPRC_OFFLINE:
    {
        enable_getrecord_ = false;
        break;
    }
    default:
        break;
    }
}

void ResumePushHandle::DisConnecting(){
    enable_getrecord_ = false;
}

void ResumePushHandle::PostResumeReq(){
    if (!queue_layer_)
        return;

    MessageData::Ptr msg_data;
    msg_data.reset(new Stanza(RES_RESUME_TASK_EVENT, session_id_));
    queue_layer_->PostDelayed(10000
                             , queue_layer_
                             , session_id_
                             , msg_data);
}

void ResumePushHandle::SetCurMaxRecordID(int cur_id){
    cur_max_record_id_ = cur_id;
}

void ResumePushHandle::SetQueueLayer(QueueLayer* queue_layer){
    queue_layer_ = queue_layer;
}

}
