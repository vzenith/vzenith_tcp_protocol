#include "VzSerialDev.h"
#include "base/logging.h"
#include "vzsdkdefines.h"
#include "vzsdkbase.h"
#include "commandanalysis.h"


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
    req_json = commandanalysis::GeneratSerialSendCmd(serial_port, data, size_data);

    // 发送485数据，不需要等返回
    if (!PostReqTask(req_json)) {
        return REQ_FAILED;
    }

    return REQ_SUCCEED;
}

int VzSerialDev::SerialStop(uint32 serial_port) {
    Json::Value req_json;
	req_json = commandanalysis::GeneratSerialStopCmd();

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