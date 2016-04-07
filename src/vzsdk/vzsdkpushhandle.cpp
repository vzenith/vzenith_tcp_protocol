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

namespace vzsdk {
PushHandle::PushHandle(const std::string &cmd_key)
    :cmd_key_(cmd_key) {
}

PushHandle::~PushHandle() {
}

//------------------------------------------------------------------------------
IvsPushHandle::IvsPushHandle(const std::string &cmd_key)
    : PushHandle(cmd_key) {
}

IvsPushHandle::~IvsPushHandle() {
}

VZ_LPRC_RESULT_TYPE IvsPushHandle::GetResultTypeFromTrigBits(unsigned uBitsTrigType) {
    if (uBitsTrigType == TRIGGER_TYPE_AUTO_BIT)
        return(VZ_LPRC_RESULT_STABLE);

    if (uBitsTrigType == TRIGGER_TYPE_EXTERNAL_BIT)
        return(VZ_LPRC_RESULT_IO_TRIGGER);

    if (uBitsTrigType == TRIGGER_TYPE_SOFTWARE_BIT)
        return(VZ_LPRC_RESULT_FORCE_TRIGGER);

    if (uBitsTrigType == TRIGGER_TYPE_VLOOP_BIT)
        return(VZ_LPRC_RESULT_VLOOP_TRIGGER);

    return(VZ_LPRC_RESULT_MULTI_TRIGGER);
}

bool IvsPushHandle::HandleMessageData(ResponseData *response) {
  std::string res_cmd = response->res_json()[JSON_REQ_CMD].asString();
  if (!(response->session_id() == this->session_id_
            && response->stanza_type() == RES_STANZA_EVENT
            && res_cmd == cmd_key()))
      return false;

    LOG(LS_INFO) << response->res_json().toStyledString();

    Json::Value _value = response->res_json();
    TH_PlateResult plate_result_;
    int nFullImgSize = 0, nClipImgSize = 0;
    commandanalysis::ParsePlateResultResponse(_value, plate_result_, nFullImgSize, nClipImgSize);

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
                         , GetResultTypeFromTrigBits(plate_result_.uBitsTrigType)
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

//------------------------------------------------------------------------------
SerialPushHandle::SerialPushHandle(const std::string &cmd_key)
    : PushHandle(cmd_key) {
    func_		= NULL;
    user_data_  = NULL;
}

SerialPushHandle::~SerialPushHandle() {
}

bool SerialPushHandle::HandleMessageData(ResponseData *response) {
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
        if (stanza_type == RES_CONNECTED_EVENT)
            conn_state_ = Socket::CS_CONNECTED;
        else if (stanza_type == RES_DISCONNECTED_EVENT_FAILURE) {
            conn_state_ = Socket::CS_CONNECTED;
        }
        if (conn_callback_) {
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

}
