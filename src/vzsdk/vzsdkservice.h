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

#ifndef SRC_HSHA_SYNC_LAYER_H_
#define SRC_HSHA_SYNC_LAYER_H_

#include "base/noncopyable.h"
#include "vzsdk/queuelayer.h"
#include "vzsdk/vzsdkpushmanager.h"
#include "vzsdkdefines.h"
#include "vzclientsdk_lpdefine.h"

namespace vzsdk {
class VzsdkService : public noncopyable {
  public:
    VzsdkService();
    virtual ~VzsdkService();

    int GetSessionID();

    void AddPushHandle(PushHandle::Ptr handle);
    void RemovePushHandle(PushHandle::Ptr handle);

    QueueLayerPtr GetQueueLayer();

    //功能模块
    VzConnectDevPtr GetConnectDev();      //设备连接
    VzRecognitionPtr GetRecongintion();   //车牌识别
    VzMaintenDevPtr GetMaintenDev();      //设备维护
    VzWlistDevPtr GetWlistDev();          //白名单
    VzSerialDevPtr GetSerialDev();        //串口
    VzIODevPtr GetIODev();                //IO

    bool Start(); //初始化
    bool Stop();  //停止服务

  protected:
    void initModule();

  private:
    QueueLayerPtr queue_layer_;
    PushManagerTask *push_manager_task_;
    ThreadPtr push_thread_;

    VzConnectDevPtr connect_dev_;
    VzRecognitionPtr recongition_;
    VzMaintenDevPtr maintenDev_;

    VzWlistDevPtr whist_dev_;
    VzSerialDevPtr serial_dev_;
    VzIODevPtr io_dev_;
};
}

#endif // SRC_HSHA_SYNC_LAYER_H_