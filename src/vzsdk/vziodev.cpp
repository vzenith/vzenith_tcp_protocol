#include "VzIODev.h"
#include "base/logging.h"
#include "vzsdkdefines.h"
#include "vzsdkbase.h"
#include "commandanalysis.h"


vzsdk::VzIODev::VzIODev(VzsdkService* _service)
    : VZModuleBase(_service) {
}


vzsdk::VzIODev::~VzIODev() {
}

int VzIODev::SetIOOutputAuto(unsigned chn_id, int duration) {
    Json::Value req_json;
    req_json = commandanalysis::GeneratSetGPIOAutoCmd(chn_id, duration);

    if ( !PostReqTask(req_json)) {
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}

int VzIODev::GetGPIOValue(int gpioIn, int *value) {
    int ret = REQ_FAILED;

    Json::Value req_json;
    req_json = commandanalysis::GeneratGetGPIOValueCmd(gpioIn);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        GPIO_RESPONSE gpio = { 0 };
        commandanalysis::ParseGPIOResponse(res_value, &gpio);

        *value = gpio.val;
        ret = REQ_SUCCEED;
    }

    return ret;
}

int VzIODev::SetOfflineCheck() {
    Json::Value req_json;
    req_json = commandanalysis::GeneratOfflineCheckCmd(2);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        OFFLINE_RESPONSE offline = { 0 };
        commandanalysis::ParseOfflineResponse(res_value, &offline);
        if (strcmp(offline.response, "ok") == 0) {
            ret = REQ_SUCCEED;
        }
    }

    return ret;
}

int VzIODev::CancelOfflineCheck() {
    Json::Value req_json;
    commandanalysis::GeneratSetOfflineCheckCmd(2, req_json);

    if (!PostReqTask(req_json)) {
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}