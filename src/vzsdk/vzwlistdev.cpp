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
#include "vzsdk/VzWlistDev.h"
#include "base/logging.h"
#include "vzsdk/vzsdkdefines.h"
#include "vzsdk/vzsdkbase.h"
#include "vzsdk/commandanalysis.h"


vzsdk::VzWlistDev::VzWlistDev(VzsdkService* service)
    : VZModuleBase(service) {
    func_		= NULL;
    user_data_  = NULL;
}


vzsdk::VzWlistDev::~VzWlistDev() {
}

int VzWlistDev::ImportWlistVehicle(VZ_LPR_WLIST_VEHICLE *wlist) {
    Json::Value req_json;
	commandanalysis::GeneratImportWlistVehicleCmd(wlist, req_json);

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
	commandanalysis::GeneratDeleteWlistVehicleCmd(plate_code, req_json);

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
	commandanalysis::GeneratQueryWlistVehicleCmd(plate_code, req_json);

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