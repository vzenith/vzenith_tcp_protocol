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

#ifndef SRC_BASE_HELPMETHODS_H_
#define SRC_BASE_HELPMETHODS_H_

#include <string>
#include "base/basictypes.h"

namespace vzsdk {

enum HttpMethod {
  HTTP_GET,
  HTTP_POST
};

class HelpMethos {
 public:
  static const std::string GetCurrentUTCTime();
  static const std::string GetSignatureNonce();
  static void Uint64ToString(uint64 n, std::string &str); // NOLINT
  static const std::string GetRandNumString(int size);
  static uint32 GetUnixTimeStamp();
  static const std::string URLEncode(const std::string &str);

  static const std::string HttpMethodToStr(HttpMethod hm);

  static void HmacSha1(const uint8 *key, std::size_t key_size,
                       const uint8 *data, std::size_t data_size, uint8 *result);

  static void HmacSha1ToBase64(const std::string &key, // NOLINT
                               const std::string &data, // NOLINT
                               std::string &result); // NOLINT
};

}  // namespace vzsdk
#endif  // SRC_BASE_HELPMETHODS_H_
