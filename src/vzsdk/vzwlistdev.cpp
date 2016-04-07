#include "VzWlistDev.h"
#include "base\logging.h"
#include "vzsdkdefines.h"
#include "vzsdkbase.h"
#include "commandanalysis.h"


vzsdk::VzWlistDev::VzWlistDev(VzsdkService* service)
    : VZModuleBase(service) {
    func_		= NULL;
    user_data_  = NULL;
}


vzsdk::VzWlistDev::~VzWlistDev() {
}

int VzWlistDev::ImportWlistVehicle(VZ_LPR_WLIST_VEHICLE *wlist) {
    Json::Value req_json;
    req_json = commandanalysis::GeneratImportWlistVehicleCmd(wlist);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        WHITE_LIST_OPERATOR_RESPONSE value = { 0 };
        commandanalysis::ParseWhiteListOperatorResponse(res_value, &value);

        if (strcmp(value.operator_type, "update_or_add") == 0 && strcmp(value.state, "succeed") == 0) {
            ret = REQ_SUCCEED;
        }
    }

    return ret;
}

int VzWlistDev::DeleteWlistVehicle(const char* plate_code) {
    Json::Value req_json;
    req_json = commandanalysis::GeneratDeleteWlistVehicleCmd(plate_code);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        WHITE_LIST_OPERATOR_RESPONSE value = { 0 };
        commandanalysis::ParseWhiteListOperatorResponse(res_value, &value);

        if (strcmp(value.operator_type, "delete") == 0 && strcmp(value.state, "succeed") == 0) {
            ret = REQ_SUCCEED;
        }
    }

    return ret;
}

int VzWlistDev::QueryWlistVehicle(const char* plate_code) {
    Json::Value req_json;
    req_json = commandanalysis::GeneratQueryWlistVehicleCmd(plate_code);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        WHITE_LIST_OPERATOR_RESPONSE value = { 0 };
        commandanalysis::ParseWhiteListOperatorResponse(res_value, &value);

        if (strcmp(value.operator_type, "select") == 0) {
            // 通过回调输出白名单查询结果
            if (func_) {
                int session_id = GetSessionID( );
                int nSize = value.record_count;
                for (int i = 0; i < nSize; i++) {
                    VZ_LPR_WLIST_VEHICLE wlist = { 0 };
                    commandanalysis::ParseWhiteListRecordResponse(res_value["dldb_rec"][i], wlist);

                    func_(session_id, user_data_, &wlist, 1);
                }
            }

            ret = REQ_SUCCEED;
        }


    }

    return ret;
}

void VzWlistDev::SetWlistRecvCallBack(VZLPRC_TCP_WLIST_RECV_CALLBACK func, void *user_data) {
    func_		= func;
    user_data_	= user_data;
}