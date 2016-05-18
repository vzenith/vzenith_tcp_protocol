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
#include "vzsdk/vzmodulebase.h"
#include "base/logging.h"
#include "vzsdk/vzconnectdev.h"

VZModuleBase::VZModuleBase(VzsdkService* sdk_service)
    : sdk_service_(sdk_service) {
}


VZModuleBase::~VZModuleBase() {
}

Message::Ptr vzsdk::VZModuleBase::SyncProcessReqTask(const Json::Value &req_json
                                                    , Thread *task_thread) {
    CritScope crit_scope(&crit_section);
    Message::Ptr msg(NULL);

    int session_id = sdk_service_->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return msg;
    }

    Task::Ptr req_task(new ReqTask(sdk_service_->GetQueueLayer().get()
                                   , DEFAULT_TIMEOUT
                                   , session_id
                                   , req_json
                                   , task_thread));

    msg = req_task->SyncProcessTask();
    return msg;
}

bool vzsdk::VZModuleBase::PostReqTask(const Json::Value &req_json) {
    CritScope crit_scope(&crit_section);
    Message::Ptr msg(NULL);
    int session_id = sdk_service_->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return false;
    }

	if (!GetIsConn())
	{
		LOG(LS_WARNING) << "The session is is disconnect, is not a right session";
		return false;
	}

    Task::Ptr req_push_task(new ReqPushTask(sdk_service_->GetQueueLayer().get(),
                                            DEFAULT_TIMEOUT,
                                            session_id,
                                            req_json));

	msg = req_push_task->SyncProcessTask();
	if (!msg || msg->phandler == NULL)
	{
		return false;
	}

    return true;
}

int vzsdk::VZModuleBase::GetSessionID() {
    CritScope crit_scope(&crit_section);
    int session_id = sdk_service_->GetSessionID();
    return session_id;
}

VzsdkService* vzsdk::VZModuleBase::getSDKService()
{
    return sdk_service_;
}

bool vzsdk::VZModuleBase::GetIsConn()
{
	Socket::ConnState conn_state = sdk_service_->GetConnectDev()->GetConnState();
	bool ret = (conn_state == 2) ? true : false;

	return ret;
}