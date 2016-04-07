#include "vzmodulebase.h"
#include "base\logging.h"

VZModuleBase::VZModuleBase(VzsdkService* _sdk_service)
    : sdk_service(_sdk_service) {
}


VZModuleBase::~VZModuleBase() {
}

Message::Ptr vzsdk::VZModuleBase::SyncProcessReqTask(const Json::Value &req_json) {
    Message::Ptr msg(NULL);

    int session_id = sdk_service->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return msg;
    }

    Task::Ptr req_task(new ReqTask(sdk_service->GetQueueLayer().get(),
                                   DEFAULT_TIMEOUT,
                                   session_id,
                                   req_json));

    msg = req_task->SyncProcessTask();
    return msg;
}

bool vzsdk::VZModuleBase::PostReqTask(const Json::Value &req_json) {
    Message::Ptr msg(NULL);

    int session_id = sdk_service->GetSessionID();
    if (session_id == 0) {
        LOG(LS_WARNING) << "The session is is zero, is not a right session";
        return false;
    }

    Task::Ptr req_task(new ReqTask(sdk_service->GetQueueLayer().get(),
                                   DEFAULT_TIMEOUT,
                                   session_id,
                                   req_json));

    req_task->PostTask();
    return true;
}

int vzsdk::VZModuleBase::GetSessionID() {
    int session_id = sdk_service->GetSessionID();
    return session_id;
}