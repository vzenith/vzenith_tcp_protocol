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
#include "vzsdk/vzrecognition.h"
#include "vzsdk/vzsdkdefines.h"
#include "vzsdk/commandanalysis.h"
#include "base/logging.h"
#include "vzsdk/vzsdkpushhandle.h"
#include "vzsdk/vzconnectdev.h"

vzsdk::VzRecognition::VzRecognition(VzsdkService* service)
    : VZModuleBase(service)
    , ivs_handle_(new vzsdk::IvsPushHandle("ivs_result"))  
    , resume_handle_(new vzsdk::ResumePushHandle("resume_rec"))
    , enable_image_(false){
    initConnect();
}


vzsdk::VzRecognition::~VzRecognition() {
}

/************************************************************************/
// 函数:     GetRecord
// 参数:     int _record_id
// 参数:     bool _need_image
// 参数:     TH_PlateResult & oPlateResult
// 返回值:   int
// 说明:     根据记录的ID，获取车牌识别记录
/************************************************************************/
int vzsdk::VzRecognition::GetRecord(int record_id, bool need_image
                                    , TH_PlateResult& plate_result
                                    , int& full_size, void* fullimage
                                    , int& clip_size, void* clipimage) {
    Json::Value req_json;
    int session_id = sdk_service_->GetSessionID();
    commandanalysis::GeneratGetRecordByIdCmd(record_id, need_image, req_json);
    std::string result;
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return SESSION_ID_INVALUE;
    }

    Task::Ptr req_task(new ReqRecordTask(sdk_service_->GetQueueLayer().get(),
                                         DEFAULT_TIMEOUT,
                                         session_id,
                                         req_json));

    Message::Ptr msg = req_task->SyncProcessTask();
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    //解析车牌
    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    Json::Value value = response->res_json();
    commandanalysis::ParsePlateResultResponse(value, plate_result, full_size, clip_size);

    int len = strlen(response->res_data().c_str());
    if (fullimage != NULL)
        memcpy(fullimage, response->res_data().c_str() + len + 1, full_size);
    if (clipimage != NULL)
        memcpy(clipimage, response->res_data().c_str() + len + full_size + 1, clip_size);

    return REQ_SUCCEED;
}

/************************************************************************/
// 函数:     GetImage
// 参数:     int _image_id
// 参数:     char * _image_ata
// 参数:     int & _image_size
// 返回值:   int
// 说明:     根据图片ID，获取图片
/************************************************************************/
int vzsdk::VzRecognition::GetImage(int image_id, char* image_data, int& image_size) {
    Json::Value req_json;
    commandanalysis::GeneratGetImageByIdCmd(image_id, req_json);

    Message::Ptr _msg = SyncProcessReqTask(req_json);
    if (!_msg || _msg->phandler == NULL) {
        return REQ_FAILED;
    }

    //解析图片
    ResponseData *response = static_cast<ResponseData *>(_msg->pdata.get());
    Json::Value result = response->res_json();

    RECORD_RESPONSE responese;
    commandanalysis::ParseRecordResponse(result, &responese);

    if (responese.id != image_id)
        return REQ_FAILED;

    int len = strlen(response->res_data().c_str());
    const char* res_data = response->res_data().c_str();
    const char* get_image_data = res_data + len + 1;
    image_size = responese.size;
    memcpy(image_data, get_image_data, responese.size);
    return REQ_SUCCEED;
}

/************************************************************************/
// 函数:     ForceTrigger
// 返回值:   int
// 说明:     强制触发
/************************************************************************/
int vzsdk::VzRecognition::ForceTrigger() {
    Json::Value req_json;
    commandanalysis::GeneratForceTrigger(req_json);

    if (!PostReqTask(req_json)) {
        return -1;
    }

    return REQ_SUCCEED;
}

int vzsdk::VzRecognition::SetReciveIvsResultCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK _func, void* _UserData, int enable_image) {
    enable_image_ = enable_image;
    static_cast<IvsPushHandle*>(ivs_handle_.get())->SetPlateCallBack(_func, _UserData);
    static_cast<IvsPushHandle*>(ivs_handle_.get())->SetSessionID(sdk_service_->GetSessionID());
    sdk_service_->AddPushHandle(ivs_handle_);
    ReciveIvsResult(sdk_service_->GetSessionID()
                    , ivs_handle_
                    , true
                    , vzsdk::FORMAT_JSON
                    , enable_image_
					, vzsdk::FULL_AND_SMALL_IMG);
    return REQ_SUCCEED;
}

void vzsdk::VzRecognition::SetResumRecordCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK func
                                                , void* user_data
                                                , bool enable_image)
{
    resume_plate_func = func;
    resume_user_data_ = user_data;
    resume_enable_image_ = enable_image;
    ResumeRecord();
}

void vzsdk::VzRecognition::GetResumRecodCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK* func
                                                 , void* user_data
                                                 , bool& enable_image)
{
    *func = resume_plate_func;
    user_data = resume_user_data_;
    resume_enable_image_ = enable_image_;
}

int vzsdk::VzRecognition::GetMaxRecordID(){
    Json::Value req_json;
    commandanalysis::GeneraGetMaxRecordID(req_json);

    Message::Ptr _msg = SyncProcessReqTask(req_json);
    if (!_msg || _msg->phandler == NULL) {
        return REQ_FAILED;
    }
    ResponseData *response = static_cast<ResponseData *>(_msg->pdata.get());
    Json::Value result = response->res_json();
    MAX_REC_RESPONSE max_rec;
    commandanalysis::ParseMaxRecResponse(result, max_rec);
    return max_rec.max_id;
}

/************************************************************************/
// 函数:     ReciveIvsResult
// 参数:     uint32 session_id
// 参数:     PushHandle::Ptr handle
// 参数:     bool enable_result
// 参数:     IvsFormat format
// 参数:     bool enable_img
// 参数:     IvsImgType img_type
// 返回值:   int
// 说明:     车牌识别结果
/************************************************************************/
int vzsdk::VzRecognition::ReciveIvsResult(uint32 session_id,
        PushHandle::Ptr handle,
        bool enable_result,
        IvsFormat format,
        bool enable_img,
        IvsImgType img_type,
        bool sync_task) {

    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return SESSION_ID_INVALUE;
    }

    Json::Value _req_json;
    commandanalysis::GeneratIVSResult(enable_result, format, enable_img, img_type, _req_json);
    sdk_service_->AddPushHandle(handle);
    Task::Ptr req_task(new ReqPushTask(sdk_service_->GetQueueLayer().get(),
                                       DEFAULT_TIMEOUT,
                                       session_id,
                                       _req_json));

    if (sync_task)
    {
        Message::Ptr msg = req_task->SyncProcessTask();
        if (!msg || msg->phandler == NULL) {
            return DEFAULT_RESULT_TIMEOUT;
        }
    }
    else
        req_task->PostTask();

    return REQ_SUCCEED;
}

/************************************************************************/
// 函数:     SlotRecvChangeConnStatus
// 参数:     int status
// 返回值:   void
// 说明:     状态改变，设置识别结果回调
/************************************************************************/
void vzsdk::VzRecognition::SlotRecvChangeConnStatus(int status)
{
    if (status == VZ_LPRC_ONLINE)
        ReciveIvsResult(sdk_service_->GetSessionID()
            , ivs_handle_
            , true
            , vzsdk::FORMAT_JSON
            , enable_image_
            , vzsdk::FULL_AND_SMALL_IMG
            , false);
}

/************************************************************************/
// 函数:     initConnect
// 返回值:   void
// 说明:     初始化信号-槽
/************************************************************************/
void vzsdk::VzRecognition::initConnect()
{
    if (sdk_service_ && sdk_service_->GetConnectDev())
    {
        VzConnectDevPtr connect_dev_ptr = sdk_service_->GetConnectDev();
        connect_dev_ptr->SignalDeviceChangeConnStatus.connect(this
                        , &VzRecognition::SlotRecvChangeConnStatus);
        connect_dev_ptr->SignalDeviceDisconnecing.connect(this
                        , &VzRecognition::SlotDisConnecting);
    }
}
/************************************************************************/
// 函数:     ResumeRecord
// 返回值:   int
// 说明:     开启车牌识别记录续传功能
/************************************************************************/
int vzsdk::VzRecognition::ResumeRecord()
{
    if (resume_handle_ == NULL)
        return REQ_FAILED;

    int max_rec_id = GetMaxRecordID();
    sdk_service_->AddPushHandle(resume_handle_);
    ResumePushHandle* handle = static_cast<ResumePushHandle*>(resume_handle_.get());
    handle->SetStartMaxRecordID(max_rec_id);
    int session_id = sdk_service_->GetSessionID();
    handle->SetSessionID(session_id);
    handle->SetQueueLayer(sdk_service_->GetQueueLayer().get());
    handle->SignalGetResumeInfo.connect(this
                               , &VzRecognition::SlotRecvGetResumeInfo);

    QueueLayer* queue_layer = sdk_service_->GetQueueLayer().get();
    MessageData::Ptr msg_data;
    msg_data.reset(new Stanza(RES_RESUME_TASK_EVENT, session_id));
    queue_layer->PostDelayed(5000, queue_layer, session_id, msg_data);

    return REQ_SUCCEED;
}

void vzsdk::VzRecognition::addRecordID(int record_id){
    static_cast<ResumePushHandle*>(resume_handle_.get())->AddRecordID(record_id);
}

void vzsdk::VzRecognition::SlotDisConnecting(){
    static_cast<ResumePushHandle*>(resume_handle_.get())->DisConnecting();
}

void vzsdk::VzRecognition::SlotRecvGetResumeInfo()
{
    ResumePushHandle* handle = static_cast<ResumePushHandle*>(resume_handle_.get());
    if (!handle)
        return;
    handle->SetCallBack(resume_plate_func, resume_user_data_);
    int cur_max_id = GetMaxRecordID();
    handle->SetCurMaxRecordID(cur_max_id);
}