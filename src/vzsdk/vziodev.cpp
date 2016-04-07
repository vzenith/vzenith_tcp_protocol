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
#include "vzsdk/vziodev.h"
#include "base/logging.h"
#include "vzsdk/vzsdkdefines.h"
#include "vzsdk/vzsdkbase.h"
#include "vzsdk/commandanalysis.h"


vzsdk::VzIODev::VzIODev(VzsdkService* _service)
    : VZModuleBase(_service) {
}


vzsdk::VzIODev::~VzIODev() {
}

int VzIODev::SetIOOutputAuto(unsigned chn_id, int duration) {
    Json::Value req_json;
	commandanalysis::GeneratSetGPIOAutoCmd(chn_id, duration, req_json);

    if ( !PostReqTask(req_json)) {
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}

int VzIODev::GetGPIOValue(int gpioIn, int *value) {
    int ret = REQ_FAILED;

    Json::Value req_json;
	commandanalysis::GeneratGetGPIOValueCmd(gpioIn, req_json);

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
	commandanalysis::GeneratOfflineCheckCmd(2, req_json);

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