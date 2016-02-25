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

namespace vzsdk {

class VzsdkServices : public noncopyable {
 public:
  VzsdkServices();
  virtual ~VzsdkServices();
  bool Start();
  bool Stop();
  //
  void AddPushHandle(PushHandle::Ptr handle);
  void RemovePushHandle(PushHandle::Ptr handle);
  //
  int ConnectServer(const std::string ip_addr, uint16 port);
  int DisconnectServer(uint32 session_id);
  int GetDeviceSN(uint32 session_id, std::string *sn);
  enum IvsFormat {
    FORMAT_BIN,
    FORMAT_JSON
  };
  enum IvsImgType {
    FULL_IMG,
    SMALL_IMG,
    FULL_AND_SMALL_IMG
  };
  int ReciveIvsResult(uint32 session_id,
                      PushHandle::Ptr handle,
                      bool enable_result,
                      IvsFormat format,
                      bool enable_img,
                      IvsImgType img_type);
 private:
  std::string IvsFormatToString(IvsFormat ivs_format);
 private:
  static const uint32 DEFAULT_TIMEOUT = 5000;
  static const uint32 FOREVER_TIMEOUT = 0XFFFF;
  static const uint32 DEFAULT_RESULT_TIMEOUT = 0;
  // return result
  static const uint32 SESSION_ID_INVALUE = -1;
  static const uint32 SESSION_NOT_FOUND = -2;
  static const uint32 UNKOWN_ERROR = -3;
  static const uint32 REQ_SUCCEED = 1;
  boost::shared_ptr<QueueLayer> queue_layer_;
  PushManagerTask *push_manager_task_;
  boost::scoped_ptr<Thread> push_thread_;
};

}

#endif // SRC_HSHA_SYNC_LAYER_H_