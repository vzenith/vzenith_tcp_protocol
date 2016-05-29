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
#ifndef SRC_HSHA_TCPDEVICEMANAGE_H_
#define SRC_HSHA_TCPDEVICEMANAGE_H_

#include <map>
#include "vzsdk/vzsdkservice.h"
#include "base/logging.h"
#include "base/scoped_ptr.h"

using namespace vzsdk;

namespace vzsdk {
typedef std::map<int, VzsdkServicesPtr> VzsdkServicesMap;

//多个设备管理
class VzTcpDeviceManage {
  public:
    VzTcpDeviceManage();
    ~VzTcpDeviceManage();
    int CreateNewService(const std::string& ip
                         , const int port
                         , const std::string& user_name
                         , const std::string& user_pwd);

    const VzsdkServicesPtr GetService(int session_id);
    const VzsdkServicesPtr GetService(const std::string& ip);

    bool ExistService(int session_id);
    bool CloseService(int session_id);

    void SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func
                                    , void *pUserData);

    void GetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func
                                    , void *pUserData);

    bool Start();

    bool Stop();

  protected:
    bool RemoveService(int session_id);

  private:
    VzsdkServicesMap vzsdk_service_map_;
    QueueLayerPtr queue_layer_;
    PushManagerTask *push_manager_task_;
    ThreadPtr push_thread_;

    CriticalSection crit_section_;

    //针对所有设备
    VZLPRC_TCP_COMMON_NOTIFY_CALLBACK conn_callback_;
    void* user_data_;
};
}

#endif


