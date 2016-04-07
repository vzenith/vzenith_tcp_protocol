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

#ifndef SRC_VZSDK_VZSDKPUSHHANDLE_H_
#define SRC_VZSDK_VZSDKPUSHHANDLE_H_

#include "base/noncopyable.h"
#include "vzsdk/queuelayer.h"
#include "vzsdk/task.h"
#include "vzlprtcpsdk.h"

namespace vzsdk {

class PushHandle : public noncopyable
    , public boost::enable_shared_from_this < PushHandle > {

  public:
    PushHandle(const std::string &cmd_key);
    virtual ~PushHandle();
    typedef boost::shared_ptr<PushHandle> Ptr;
    virtual bool HandleMessageData(ResponseData *response) = 0;
    const std::string &cmd_key() {
        return cmd_key_;
    }

  private:
    std::string cmd_key_;
};

//------------------------------------------------------------------------------
class IvsPushHandle : public PushHandle {
  public:
    IvsPushHandle(const std::string &cmd_key);
    virtual ~IvsPushHandle();

    virtual bool HandleMessageData(ResponseData *response);

    void SetPlateCallBack(VZLPRC_TCP_PLATE_INFO_CALLBACK result_callback, void* result_userdata);
    void SetSessionID(int session_id);

  private:
    VZ_LPRC_RESULT_TYPE GetResultTypeFromTrigBits(unsigned uBitsTrigType);
    VZLPRC_TCP_PLATE_INFO_CALLBACK result_callback_;
    void* result_userdata_;

    int session_id_;
};

//------------------------------------------------------------------------------
class SerialPushHandle : public PushHandle {
  public:
    SerialPushHandle(const std::string &cmd_key);
    virtual ~SerialPushHandle();
    virtual bool HandleMessageData(ResponseData *response);
    void SetSerialRecvCallBack(VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func, void *user_data);

  private:
    VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func_;
    void *user_data_;
};

}

#endif // SRC_VZSDK_VZSDKPUSHMANAGER_H_