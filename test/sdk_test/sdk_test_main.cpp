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

#include <iostream>
#include "base/logging.h"
#include "vzsdk/vzsdkservices.h"
#include <sstream>

int main(void) {
  vzsdk::VzsdkServices vzsdk_services;
  vzsdk_services.Start();
  std::string sn;
  int session_id = vzsdk_services.ConnectServer("192.168.4.78", 8131);
  LOG(LS_INFO) << "session_id = " << session_id;

  vzsdk::IvsPushHandle::Ptr ivs_handle(
    new vzsdk::IvsPushHandle("ivs_result"));
  vzsdk_services.GetDeviceSN(session_id, &sn);
  vzsdk_services.ReciveIvsResult(session_id,
                                 ivs_handle,
                                 true,
                                 vzsdk::VzsdkServices::FORMAT_JSON,
                                 true,
                                 vzsdk::VzsdkServices::FULL_IMG);
  vzsdk_services.GetDeviceSN(session_id, &sn);

  //if(vzsdk_services.GetDeviceSN(session_id, &sn) < 0) {
  //  LOG(LS_ERROR) << "Get device sn number getting an error";
  //} else {
  //  LOG(LS_INFO) << "The Device SN Number is " << sn;
  //}
  // vzsdk_services.DisconnectServer(session_id);
  // LOG(LS_INFO) << "session disconnected";
#ifdef WIN32
  Sleep(1000000);
#endif
  return EXIT_SUCCESS;
}