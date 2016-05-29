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
#ifndef SRC_HSHA_WLIST_DEV_H_
#define SRC_HSHA_WLIST_DEV_H_

#include "vzsdk/queuelayer.h"
#include "vzsdk/vzlprtcpsdk.h"
#include "vzsdk/vzmodulebase.h"

using namespace vzsdk;

namespace vzsdk {
class VzWlistDev : public VZModuleBase {
  public:
    VzWlistDev(VzsdkService* _service);
    ~VzWlistDev();

    int ImportWlistVehicle(VZ_LPR_WLIST_VEHICLE *wlist);

    int DeleteWlistVehicle(const char* plate_code);

    int QueryWlistVehicle(const char* plate_code);

    void SetWlistRecvCallBack(VZLPRC_TCP_WLIST_RECV_CALLBACK func, void *user_data);

  private:
    VZLPRC_TCP_WLIST_RECV_CALLBACK func_;
    void *user_data_;
};
}

#endif