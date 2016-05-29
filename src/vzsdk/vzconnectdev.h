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
#ifndef SRC_HSHA_SYNC_CONNECTDEV_H_
#define SRC_HSHA_SYNC_CONNECTDEV_H_

#include "vzsdk/queuelayer.h"
#include "vzsdk/vzsdkservice.h"
#include "vzsdk/vzmodulebase.h"

namespace vzsdk {
class  VzsdkService;

class VzConnectDev : public VZModuleBase
                   , public sigslot::has_slots<>{
  public:
    VzConnectDev(VzsdkService* _service);
    ~VzConnectDev();

    sigslot::signal1<int> SignalDeviceChangeConnStatus; //连接状态改变
    sigslot::signal0<> SignalDeviceDisconnecing;        //正在断开连接

    int ConnectServer(const std::string& _ip_addr, uint16 _port);
    int DisconnectServer();

    std::string GetIP();
    int GetSessionID();
    Socket::ConnState GetConnState();

    void SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func
                                 , void *pUserData);

  protected:
    int ReConnectServer();
    void ChangeConn();

    void SlotChangeConnStatus(int);

  private:
    Task::Ptr connect_task_;
    ReqConnectDataPtr req_connect_data_ptr_;
    ChangeConnPushHandle::Ptr change_conn_pushhandle;
};
}

#endif

