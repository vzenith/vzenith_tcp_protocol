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
#ifndef SRC_HSHA_RECONGINITION_H_
#define SRC_HSHA_RECONGINITION_H_

#include "vzsdk\queuelayer.h"
#include "vzclientsdk_lpdefine.h"
#include "vzsdkpushhandle.h"
#include "vzsdkdefines.h"
#include "vzlprtcpsdk.h"
#include "vzsdkservice.h"
#include "vzmodulebase.h"

using namespace vzsdk;
namespace vzsdk {
class VzsdkService;

class VzRecognition : public VZModuleBase {
  public:
    explicit VzRecognition(VzsdkService* service);
    ~VzRecognition();

    int GetRecord(int record_id, bool need_image, TH_PlateResult& oPlateResult);
    int GetImage(int image_id, char* image_ata, int& image_size);
    int ForceTrigger();
    int setReciveIvsResultCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK func,
                                   void *pUserData,
                                   int bEnableImage);

  private:
    int ReciveIvsResult(uint32 session_id,
                        PushHandle::Ptr handle,
                        bool enable_result,
                        IvsFormat format,
                        bool enable_img,
                        IvsImgType img_type);

  private:
    vzsdk::IvsPushHandle::Ptr ivs_handle_;
};
}

#endif