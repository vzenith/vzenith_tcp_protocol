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

#include "vzsdk/queuelayer.h"
#include "vzsdk/vzclientsdk_lpdefine.h"
#include "vzsdk/vzsdkpushhandle.h"
#include "vzsdk/vzsdkdefines.h"
#include "vzsdk/vzlprtcpsdk.h"
#include "vzsdk/vzsdkservice.h"
#include "vzsdk/vzmodulebase.h"

using namespace vzsdk;
namespace vzsdk {
class VzsdkService;

class VzRecognition : public VZModuleBase 
                    , public sigslot::has_slots<>{
  public:
    explicit VzRecognition(VzsdkService* service);
    ~VzRecognition();

    int GetRecord(int record_id, bool need_image
                    , TH_PlateResult& plate_result
                    , int& full_size, void* fullimage
                    , int& clip_size, void* clipimage);
    int GetImage(int image_id, char* image_ata, int& image_size);
    int ForceTrigger();
    int SetReciveIvsResultCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK func,
                                   void *pUserData,
                                   int enable_Image);

    void SetResumRecordCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK func, void* user_data, bool EnableImage = true);
    void GetResumRecodCallback(VZLPRC_TCP_PLATE_INFO_CALLBACK* func, void* user_data, bool& enable_image);
    int GetMaxRecordID();
	void addRecordID(int record_id);

  protected:
    void initConnect();
    int ReciveIvsResult(uint32 session_id,
                        PushHandle::Ptr handle,
                        bool enable_result,
                        IvsFormat format,
                        bool enable_img,
                        IvsImgType img_type,
                        bool sync_task = true);
    
 private:
    int ResumeRecord();
    
    //slots
    void SlotRecvChangeConnStatus(int);
    void SlotDisConnecting();
    void SlotRecvGetResumeInfo();

private:
    vzsdk::IvsPushHandle::Ptr ivs_handle_;
    bool enable_image_;
    vzsdk::ResumePushHandle::Ptr resume_handle_;
    bool resume_enable_image_;

    //Ðø´«
    VZLPRC_TCP_PLATE_INFO_CALLBACK resume_plate_func;
    void* resume_user_data_;
};
}

#endif