#include "vzsdk/vzlprtcpsdk.h"
#include "base/criticalsection.h"
#include "base/vzfile.h"

#include "vztcpdevicemanage.h"
#include "vzsdk/vzsdkdefines.h"

#include "vzconnectdev.h"
#include "vzrecognition.h"
#include "vzmaintendev.h"
#include "vzserialdev.h"
#include "vzwlistdev.h"
#include "vziodev.h"
#include <string.h>
#include "base/socket.h"

using namespace std;
using namespace vzsdk;
vzsdk::CriticalSection scoped_lock;
VzTcpDeviceManage* vztcp_device_manage = NULL;

const int _vzsdk_success  = 0;
const int _vzsdk_failed   = -1;

VZ_LPRC_LASTERROR_TYPE g_LastError;
void SetLastError(int ret, VZ_LPRC_LASTERROR_TYPE eLastErrorType)
{
  if (ret != _vzsdk_success)
    g_LastError = eLastErrorType;
}

bool CheckManage()
{
  int ret = vztcp_device_manage != NULL ? _vzsdk_success : _vzsdk_failed;
  SetLastError(ret, VZ_LPRC_LASTERROR_NOT_SETUP);
  return ret == _vzsdk_success;
}

bool CheckDevice(VzLPRTcpHandle handle)
{
  bool exist_manage = CheckManage();
  bool ret = exist_manage;
  if (exist_manage)
  {
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres)
      ret = true;
    else
      SetLastError(_vzsdk_failed, VZ_LPRC_LASTERROR_INVALID_HANDLE);
  }
  return ret;
}

int __STDCALL VzLPRTcp_Setup() {
    CritScope _critscope(&scoped_lock);

    int _ret = _vzsdk_failed;

    vztcp_device_manage = new VzTcpDeviceManage;
    if (vztcp_device_manage)
        _ret = _vzsdk_success;

    return _ret;
}

void __STDCALL VzLPRTcp_Cleanup() {
    CritScope _critscope(&scoped_lock);

    if (vztcp_device_manage)
        delete vztcp_device_manage;
}

VzLPRTcpHandle __STDCALL VzLPRTcp_Open(const char *pStrIP, WORD wPort, const char *pStrUserName, const char *pStrPassword) {
    int ret = _vzsdk_failed;
    ret = vztcp_device_manage->CreateNewService(pStrIP, wPort, pStrUserName, pStrPassword);
    SetLastError(ret, VZ_LPRC_LASTERROR_CREATE_FAILED);
    return ret > 0 ? ret : 0;
}

int __STDCALL VzLPRTcp_Close(VzLPRTcpHandle handle) {
    int ret = _vzsdk_failed;
    if (!CheckDevice(handle))
      return ret;

    ret = vztcp_device_manage->CloseService(handle) ? _vzsdk_success : _vzsdk_failed;
    return ret;
}

int __STDCALL VzLPRTcp_CloseByIP(const char *pStrIP) {
  int ret = _vzsdk_failed;

  VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(pStrIP);
  if (_serveres) {
    vzsdk::VzConnectDevPtr _connect_ptr = _serveres->GetConnectDev();
    VzLPRTcpHandle _session_id = _connect_ptr->GetSessionID();
    if (_session_id != NULL_SESSION_ID)
      ret = VzLPRTcp_Close(_session_id);
  }
  SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_IP);
  return ret;
}

int __STDCALL VzLPRTcp_GetDeviceSN(VzLPRTcpHandle handle, char* pSN) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

  if (pSN == NULL)
  {
    SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_PARAM);
    return ret;
  }

  VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
  if (_serveres) {
    string _device_sn;
    VzMaintenDevPtr _maintendev = _serveres->GetMaintenDev();
    ret = _maintendev->GetDeviceSN(_device_sn);
    if (ret == _vzsdk_success)
      memcpy(pSN, _device_sn.c_str(), _device_sn.length());
  }
  SetLastError(ret, VZ_LPRC_LASTERROR_GETDEVICE_ERROR);
  return ret;
}

int __STDCALL VzLPRTcp_IsConnected(VzLPRTcpHandle handle, BYTE *pStatus) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

  if (pStatus == NULL)
  {
    SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_PARAM);
    return ret;
  }

  VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
  if (_serveres) {
    int _conn_state = _serveres->GetConnectDev()->GetConnState();
    *pStatus = (_conn_state == 2) ? true : false;
    ret = _vzsdk_success;
  }
  SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_PSTATUS);
  return ret;
}

int __STDCALL VzLPRTcp_SetPlateInfoCallBack(VzLPRTcpHandle handle
        , VZLPRC_TCP_PLATE_INFO_CALLBACK func
        , void *pUserData
        , int bEnableImage) {
  
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzRecognitionPtr _recongnition_ptr = _serveres->GetRecongintion();
        if (_recongnition_ptr) {
            _recongnition_ptr->setReciveIvsResultCallback(func, pUserData, bEnableImage);
        }
    }
    return ret;
}

int __STDCALL VzLPRTcp_ImageSaveToJpeg(unsigned char *pImgBuf
            , int nImgSize
            , const char *pSavePathName
            , int nQuality) {

    int ret = _vzsdk_failed;
    if (pImgBuf == NULL || nImgSize <= 0 || pSavePathName == NULL || strlen(pSavePathName) == 0) {
      SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_PARAM);
       return _vzsdk_failed;
    }

    int _ret = _vzsdk_failed;

    bool _write_ret = CVzFile::WriteFile(pSavePathName, pImgBuf, nImgSize);
    _ret = _write_ret ? _vzsdk_success : _vzsdk_failed;
    return _ret;
}

int __STDCALL VzLPRTcp_ForceTrigger(VzLPRTcpHandle handle) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzRecognitionPtr _reconginition_ptr = _serveres->GetRecongintion();
        ret = _reconginition_ptr->ForceTrigger();
    }
    return ret;
}

int __STDCALL VzLPRTcp_GetLastError()
{
  return (int)g_LastError;
}

int __STDCALL VzLPRTcp_LoadRecordById(VzLPRTcpHandle handle, int id, TH_PlateResult *pResult) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzRecognitionPtr _reconginition_ptr = _serveres->GetRecongintion();
        ret = _reconginition_ptr->GetRecord(id, false, *pResult);
    }
    else
      SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_HANDLE);

    return ret;
}

const char* __STDCALL VzLPRTcp_GetIP(VzLPRTcpHandle handle) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return NULL;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        static std::string _device_ip = _serveres->GetConnectDev()->GetIP();
        return _device_ip.c_str();
    }
    return NULL;
}

int __STDCALL VzLPRTcp_SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *pUserData)
{

  return _vzsdk_success;
}


int __STDCALL VzLPRTcp_LoadImageById(VzLPRTcpHandle handle, int id, void *pdata, int* size) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;
  if (pdata == NULL || size == NULL)
  {
    SetLastError(ret, VZ_LPRC_LASTERROR_INVALID_PARAM);
    return ret;
  }
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzRecognitionPtr _reconginition_ptr = _serveres->GetRecongintion();
        ret = _reconginition_ptr->GetImage(id, (char*)pdata, *size);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SerialStart(VzLPRTcpHandle handle, int nSerialPort,
                                   VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func, void *pUserData) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzSerialDevPtr _serial_ptr = _serveres->GetSerialDev();
        ret = _serial_ptr->SerialStart(nSerialPort);
		_serial_ptr->SetSerialRecvCallBack(func, pUserData);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SerialSend(VzLPRTcpHandle handle, int nSerialPort, const unsigned char *pData, unsigned uSizeData) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzSerialDevPtr _serial_ptr = _serveres->GetSerialDev();
        ret = _serial_ptr->SerialSend(nSerialPort, pData, uSizeData);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SerialStop(VzLPRTcpHandle handle, int nSerialPort) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzSerialDevPtr _serial_ptr = _serveres->GetSerialDev();
        ret = _serial_ptr->SerialStop(nSerialPort);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SetIOOutputAuto(VzLPRTcpHandle handle, unsigned uChnId, int nDuration) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzIODevPtr _io_ptr = _serveres->GetIODev();
        ret = _io_ptr->SetIOOutputAuto(uChnId, nDuration);
    }
    return ret;
}

int __STDCALL VzLPRTcp_GetGPIOValue(VzLPRTcpHandle handle, int gpioIn, int *value) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzIODevPtr _io_ptr = _serveres->GetIODev();
        ret = _io_ptr->GetGPIOValue(gpioIn, value);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SetOfflineCheck(VzLPRTcpHandle handle) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzIODevPtr _io_ptr = _serveres->GetIODev();
        ret = _io_ptr->SetOfflineCheck();
    }
    return ret;
}

int __STDCALL VzLPRTcp_CancelOfflineCheck(VzLPRTcpHandle handle) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzIODevPtr _io_ptr = _serveres->GetIODev();
        ret = _io_ptr->CancelOfflineCheck();
    }
    return ret;
}

int __STDCALL VzLPRTcp_ImportWlistVehicle(VzLPRTcpHandle handle, VZ_LPR_WLIST_VEHICLE *pWlist) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzWlistDevPtr _wlist_ptr = _serveres->GetWlistDev();
        ret = _wlist_ptr->ImportWlistVehicle(pWlist);
    }
    return ret;
}

int __STDCALL VzLPRTcp_DeleteWlistVehicle(VzLPRTcpHandle handle, const char* plateCode) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;

    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzWlistDevPtr _wlist_ptr = _serveres->GetWlistDev();
        ret = _wlist_ptr->DeleteWlistVehicle(plateCode);
    }
    return ret;
}

int __STDCALL VzLPRTcp_QueryWlistVehicle(VzLPRTcpHandle handle, const char* plateCode) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzWlistDevPtr _wlist_ptr = _serveres->GetWlistDev();
        ret = _wlist_ptr->QueryWlistVehicle(plateCode);
    }
    return ret;
}

int __STDCALL VzLPRTcp_SetWlistInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_WLIST_RECV_CALLBACK func, void *pUserData) {
  int ret = _vzsdk_failed;
  if (!CheckDevice(handle))
    return ret;
    VzsdkServicesPtr _serveres = vztcp_device_manage->GetService(handle);
    if (_serveres) {
        VzWlistDevPtr _wlist_ptr = _serveres->GetWlistDev();
		_wlist_ptr->SetWlistRecvCallBack(func, pUserData);
    }
    return ret;
}