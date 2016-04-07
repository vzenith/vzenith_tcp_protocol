#include "VzTcpDeviceManage.h"
#include "VzConnectDev.h"

const int _c_success = 0;
const int _c_failed = -1;

vzsdk::VzTcpDeviceManage::VzTcpDeviceManage() {
}

/************************************************************************/
// 函数:     ~VzTcpDeviceManage
// 返回值:
// 说明:     析构，释放资源
/************************************************************************/
vzsdk::VzTcpDeviceManage::~VzTcpDeviceManage() {
}

/************************************************************************/
// 函数:     ExistService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     是否存在session_id对应的服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::ExistService(int session_id) {
    bool bExist = true;
    VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
    if (it == vzsdk_service_map_.end())
        bExist = false;
    return bExist;
}

/************************************************************************/
// 函数:     GetService
// 参数:     int _session_id
// 返回值:   const VzsdkServicesPtr
// 说明:     根据session_id获取到服务
/************************************************************************/
const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(int session_id) {
    VzsdkServicesPtr vzsdk_service;
    if (ExistService(session_id)) {
        VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
        vzsdk_service = it->second;
    }
    return vzsdk_service;
}

const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(const std::string& ip) {
    VzsdkServicesPtr vzsdk_service;
    for (VzsdkServicesMap::iterator it = vzsdk_service_map_.begin();
            it != vzsdk_service_map_.end();
            ++it) {
        VzConnectDevPtr _connect_ptr = it->second->GetConnectDev();
        if (ip == _connect_ptr->GetIP()) {
            vzsdk_service = it->second;
            break;
        }
    }

    return vzsdk_service;
}

/************************************************************************/
// 函数:     RemoveService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     从映射表中移除服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::RemoveService(int session_id) {
    bool bRet = false;
    VzsdkServicesMap::iterator it = vzsdk_service_map_.find(session_id);
    if (it != vzsdk_service_map_.end()) {
        vzsdk_service_map_.erase(it);
        bRet = true;
    }

    return bRet;
}

/************************************************************************/
// 函数:     CreateNewService
// 参数:     const std::string & _ip
// 参数:     const int _port
// 参数:     const std::string & _user_name
// 参数:     const std::string & _user_pwd
// 返回值:   int
// 说明:     创建新的服务
/************************************************************************/
int vzsdk::VzTcpDeviceManage::CreateNewService(const std::string& ip, const int port, const std::string& user_name, const std::string& user_pwd) {
    VzsdkServicesPtr sdk_service(new VzsdkService);
    sdk_service->Start();
    int session_id = sdk_service->GetConnectDev()->ConnectServer(ip, port);
    if (session_id != SESSION_ID_INVALUE) {
        vzsdk_service_map_.insert(std::make_pair(session_id, sdk_service));
    }
    return session_id;
}

/************************************************************************/
// 函数:     CloseService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     关闭服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::CloseService(int session_id) {
    if (!ExistService(session_id))
        return false;

    VzsdkServicesPtr sdk_service = GetService(session_id);
    if (sdk_service) {
        sdk_service->Stop();
        RemoveService(session_id);
    }
    return true;
}
