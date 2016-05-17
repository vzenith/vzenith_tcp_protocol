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

#ifndef SRC_VZSDK_VZSDKBASE_H_
#define SRC_VZSDK_VZSDKBASE_H_

namespace vzsdk {

#define JSON_REQ_CMD "cmd"

//
#define JSON_REQ_CMD_FORCETRIGGER "trigger"
#define JSON_REQ_CMD_GETSN "getsn"
#define JSON_PUSH_CMD_IVSRESULT "ivsresult"
#define  JSON_REQ_CMD_GETMAXRECORDID "get_max_rec_id"

#define JSON_REQ_IVSRESULT_ENABLE "enable"
#define JSON_REQ_IVSRESULT_FORMAT "format"
#define JSON_REQ_IVSRESULT_ENABLE_IMAGE "image"
#define JSON_REQ_IVSRESULT_IMAGE_TYPE "image_type"

#define JSON_VALUE "value"

}

#endif // SRC_VZSDK_VZSDKBASE_H_