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
    for (VzsdkServicesMap::iterator it = vzsdk_service_map.begin();
            it != vzsdk_service_map.end();
            ++it) {
        //it->second->GetConnectDev()->DisconnectServer();
    }
    vzsdk_service_map.clear();
}

/************************************************************************/
// 函数:     ExistService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     是否存在session_id对应的服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::ExistService(int _session_id) {
    bool bExist = true;
    VzsdkServicesMap::iterator it = vzsdk_service_map.find(_session_id);
    if (it == vzsdk_service_map.end())
        bExist = false;
    return bExist;
}

/************************************************************************/
// 函数:     GetService
// 参数:     int _session_id
// 返回值:   const VzsdkServicesPtr
// 说明:     根据session_id获取到服务
/************************************************************************/
const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(int _session_id) {
    VzsdkServicesPtr _vzsdk_service;
    if (ExistService(_session_id)) {
        VzsdkServicesMap::iterator it = vzsdk_service_map.find(_session_id);
        _vzsdk_service = it->second;
    }
    return _vzsdk_service;
}

const VzsdkServicesPtr vzsdk::VzTcpDeviceManage::GetService(const std::string& _ip) {
    VzsdkServicesPtr _vzsdk_service;
    for (VzsdkServicesMap::iterator it = vzsdk_service_map.begin();
            it != vzsdk_service_map.end();
            ++it) {
        VzConnectDevPtr _connect_ptr = it->second->GetConnectDev();
        if (_ip == _connect_ptr->GetIP()) {
            _vzsdk_service = it->second;
            break;
        }
    }

    return _vzsdk_service;
}

/************************************************************************/
// 函数:     RemoveService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     从映射表中移除服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::RemoveService(int _session_id) {
    bool bRet = false;
    VzsdkServicesMap::iterator it = vzsdk_service_map.find(_session_id);
    if (it != vzsdk_service_map.end()) {
        vzsdk_service_map.erase(it);
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
int vzsdk::VzTcpDeviceManage::CreateNewService(const std::string& _ip, const int _port, const std::string& _user_name, const std::string& _user_pwd) {
    VzsdkServicesPtr _sdk_service(new VzsdkService);
    _sdk_service->Start();
    int _session_id = _sdk_service->GetConnectDev()->ConnectServer(_ip, _port);
    if (_session_id != SESSION_ID_INVALUE) {
        vzsdk_service_map.insert(std::make_pair(_session_id, _sdk_service));
    }
    return _session_id;
}

/************************************************************************/
// 函数:     CloseService
// 参数:     int _session_id
// 返回值:   bool
// 说明:     关闭服务
/************************************************************************/
bool vzsdk::VzTcpDeviceManage::CloseService(int _session_id) {
    if (!ExistService(_session_id))
        return false;

    VzsdkServicesPtr _sdk_service = GetService(_session_id);
    if (_sdk_service) {
        _sdk_service->Stop();
        RemoveService(_session_id);
    }
    return true;
}
