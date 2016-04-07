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
#ifndef SRC_VZSDK_VZSDKDEFINES_H_
#define SRC_VZSDK_VZSDKDEFINES_H_

#include "base/shared_ptr.h"

namespace vzsdk {

typedef int SessionID;

class VzsdkService;
typedef boost::shared_ptr<VzsdkService> VzsdkServicesPtr;

class VzConnectDev;
typedef boost::shared_ptr<VzConnectDev> VzConnectDevPtr;

class VzRecognition;
typedef boost::shared_ptr<VzRecognition> VzRecognitionPtr;

class VzMaintenDev;
typedef boost::shared_ptr<VzMaintenDev> VzMaintenDevPtr;

class VzWlistDev;
typedef boost::shared_ptr<VzWlistDev> VzWlistDevPtr;

class VzSerialDev;
typedef boost::shared_ptr<VzSerialDev> VzSerialDevPtr;

class VzIODev;
typedef boost::shared_ptr<VzIODev> VzIODevPtr;

//连接常量定义
static const int32 DEFAULT_RESULT_TIMEOUT = 0;
static const int32 DEFAULT_TIMEOUT        = 5000;
static const int32 FOREVER_TIMEOUT        = 0XFFFF;

//接口返回值定义
static const int32 REQ_SUCCEED        = 0;
static const int32 SESSION_ID_INVALUE = -1;
static const int32 SESSION_NOT_FOUND  = -2;
static const int32 UNKOWN_ERROR       = -3;
static const int32 REQ_FAILED         = -4;

//识别结果格式
static const std::string IVSFormat_BIN  = "bin";
static const std::string IVSFormat_JSON = "json";

enum IvsFormat {
    FORMAT_BIN,         //二进制
    FORMAT_JSON         //json格式
};

//识别结果所带图片类型
enum IvsImgType {
    FULL_IMG,           //大图
    SMALL_IMG,          //小图
    FULL_AND_SMALL_IMG  //大图和小图
};

}

#endif