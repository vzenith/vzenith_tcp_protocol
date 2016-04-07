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
#include "vzsdk/VzSerialDev.h"
#include "base/logging.h"
#include "vzsdk/vzsdkdefines.h"
#include "vzsdk/vzsdkbase.h"
#include "vzsdk/commandanalysis.h"


vzsdk::VzSerialDev::VzSerialDev(VzsdkService* _service)
    : VZModuleBase(_service) {
}


vzsdk::VzSerialDev::~VzSerialDev() {
}

int VzSerialDev::SerialStart(uint32 serial_port) {
    Json::Value req_json;
	if (!commandanalysis::GeneratSerialStartCmd(serial_port, req_json))
	{
		return REQ_FAILED;
	}

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        TTRANSMISSION_RESPONSE value = { 0 };
        commandanalysis::ParseTTransmissionResponse(res_value, &value);

        if (strcmp(value.subcmd,"init") == 0 && strcmp(value.response, "ok") == 0) {
            ret = REQ_SUCCEED;
        }
    }

    return ret;
}

int VzSerialDev::SerialSend(uint32 serial_port, const unsigned char *data, unsigned size_data) {
    Json::Value req_json;
	if (commandanalysis::GeneratSerialSendCmd(serial_port, data, size_data, req_json))
	{
		return REQ_FAILED;
	}

    // 发送485数据，不需要等返回
    if (!PostReqTask(req_json)) {
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}

int VzSerialDev::SerialStop() {
    Json::Value req_json;
	commandanalysis::GeneratSerialStopCmd(req_json);

    Message::Ptr msg = SyncProcessReqTask(req_json);
    if (!msg || msg->phandler == NULL) {
        return REQ_FAILED;
    }

    int ret = REQ_FAILED;

    ResponseData *response = static_cast<ResponseData *>(msg->pdata.get());
    if (response != NULL) {
        Json::Value res_value = response->res_json();

        TTRANSMISSION_RESPONSE value = { 0 };
        commandanalysis::ParseTTransmissionResponse(res_value, &value);

        if (strcmp(value.subcmd, "uninit") == 0 && strcmp(value.response, "ok") == 0) {
            ret = REQ_SUCCEED;
        }
    }

    return ret;
}

void VzSerialDev::SetSerialRecvCallBack(VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func, void *user_data) {
	vzsdk::SerialPushHandle::Ptr serial_handle(new SerialPushHandle("ttransmission"));
	static_cast<SerialPushHandle*>(serial_handle.get())->SetSerialRecvCallBack(func, user_data);
	sdk_service_->AddPushHandle(serial_handle);
}