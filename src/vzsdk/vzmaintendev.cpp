#include "VzMaintenDev.h"
#include "base\logging.h"
#include "vzsdkdefines.h"
#include "vzsdkbase.h"
#include "commandanalysis.h"


vzsdk::VzMaintenDev::VzMaintenDev(VzsdkService* _service)
    : VZModuleBase(_service) {
}


vzsdk::VzMaintenDev::~VzMaintenDev() {
}

int VzMaintenDev::GetDeviceSN(std::string &sn) {
    Json::Value req_json;
    commandanalysis::GeneratGetDeviceSN(sdk_service_->GetSessionID(), req_json);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    sn = response->res_json()[JSON_VALUE].asString();
    return REQ_SUCCEED;
}