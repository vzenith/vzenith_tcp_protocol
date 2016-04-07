#include "vzmodulebase.h"
#include "base/logging.h"

VZModuleBase::VZModuleBase(VzsdkService* sdk_service)
    : sdk_service_(sdk_service) {
}


VZModuleBase::~VZModuleBase() {
}

Message::Ptr vzsdk::VZModuleBase::SyncProcessReqTask(const Json::Value &req_json) {
    Message::Ptr msg(NULL);

    int session_id = sdk_service_->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return msg;
    }

    Task::Ptr req_task(new ReqTask(sdk_service_->GetQueueLayer().get(),
                                   DEFAULT_TIMEOUT,
                                   session_id,
                                   req_json));

    msg = req_task->SyncProcessTask();
    return msg;
}

bool vzsdk::VZModuleBase::PostReqTask(const Json::Value &req_json) {
    Message::Ptr msg(NULL);

    int session_id = sdk_service_->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return false;
    }

    Task::Ptr req_push_task(new ReqPushTask(sdk_service_->GetQueueLayer().get(),
                                            DEFAULT_TIMEOUT,
                                            session_id,
                                            req_json));

    req_push_task->SyncProcessTask();
    return true;
}

int vzsdk::VZModuleBase::GetSessionID() {
    int session_id = sdk_service_->GetSessionID();
    return session_id;
}