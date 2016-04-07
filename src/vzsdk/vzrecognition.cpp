#include "VzRecognition.h"
#include "vzsdk\vzsdkdefines.h"
#include "commandanalysis.h"
#include "base\logging.h"
#include "vzsdkpushhandle.h"

vzsdk::VzRecognition::VzRecognition(VzsdkService* service)
    : VZModuleBase(service)
    , ivs_handle_(new vzsdk::IvsPushHandle("ivs_result")) {
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
int vzsdk::VzRecognition::GetRecord(int record_id, bool need_image, TH_PlateResult& oPlateResult) {
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
    TH_PlateResult plate_result;
    int nFullImgSize = 0, nClipImgSize = 0;
    commandanalysis::ParsePlateResultResponse(value, plate_result, nFullImgSize, nClipImgSize);

    void *pImage = NULL;
    void *pClipImage = NULL;

    int len = strlen(response->res_data().c_str());
    pImage = (void *)(response->res_data().c_str() + len + 1);
    pClipImage = (void *)(response->res_data().c_str() + len + nFullImgSize + 1);

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
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}

int vzsdk::VzRecognition::setReciveIvsResultCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK _func, void* _UserData, int bEnableImage) {
    static_cast<IvsPushHandle*>(ivs_handle_.get())->SetPlateCallBack(_func, _UserData);
    sdk_service_->AddPushHandle(ivs_handle_);
    ReciveIvsResult(sdk_service_->GetSessionID()
                    , ivs_handle_
                    , true
                    , vzsdk::FORMAT_JSON
                    , bEnableImage
                    , vzsdk::FULL_IMG);
    return REQ_SUCCEED;
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
        IvsImgType img_type) {

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

    Message::Ptr msg = req_task->SyncProcessTask();
    if (!msg || msg->phandler == NULL) {
        return DEFAULT_RESULT_TIMEOUT;
    }
    return REQ_SUCCEED;
}