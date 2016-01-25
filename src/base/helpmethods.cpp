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

#include "base/helpmethods.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include "base/sha1.h"
#include "base/base64.h"
#include "base/timeutils.h"

namespace vzsdk {

const std::string HelpMethos::GetCurrentUTCTime() {
  // temp buffer
  static const int MAX_UTC_TIME_STR_SIZE = 32;
  char temp_buf[MAX_UTC_TIME_STR_SIZE];
  static int CN_UTC_TIMEZONE = 8 * 60 * 60;

  // Get current time
  tm tt_tm;
  int mic_seconds = 0;
  CurrentTmTime(&tt_tm, &mic_seconds);

  sprintf(temp_buf, "%04d-%02d-%02dT%02d:%02d:%02dZ", // NOLINT
          tt_tm.tm_year + 1900,
          tt_tm.tm_mon + 1,
          tt_tm.tm_mday,
          tt_tm.tm_hour,
          tt_tm.tm_min,
          tt_tm.tm_sec);

  return temp_buf;
}

const std::string HelpMethos::GetSignatureNonce() {
  std::string signature;
  Uint64ToString(time(NULL), signature);
  return signature + GetRandNumString(4);
}

void HelpMethos::Uint64ToString(uint64 n, std::string &str) {
  if (n == 0) {
    str.push_back('0');
    return;
  }
  while (n) {
    str.push_back((n % 10) + '0');
    n = n / 10;
  }
}

const std::string HelpMethos::GetRandNumString(int size) {
  static const int MAX_NUM_SET_SIZE = 10;
  std::string res_str;
  // Setting the random seed
  srand((unsigned int)TimeNanos());
  for (int i = 0; i < size; i++) {
    res_str.push_back((rand() % MAX_NUM_SET_SIZE) + '0'); // NOLINT
  }
  return res_str;
}

uint32 HelpMethos::GetUnixTimeStamp() {
  return static_cast<uint32>(TimeNanos() / 1000);
}

static const char HEX[] = "0123456789ABCDEF";

#define TO_HEX(x) ((x) < 16) ? HEX[(x)] : '!'

// 0-33 "#$%&+,/:;<=>?@[\]^`{|} 127
const unsigned char URL_UNSAFE = 0x1;

// ! " # $ % & ' ( ) * + , - . / 0 1 2 3 4 6 5 7 8 9 : ; < = > ?
// @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _
// ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~

const unsigned char ASCII_CLASS[128] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // NOLINT
  1, 0, 3, 1, 1, 1, 3, 2, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 1, 3, 1, // NOLINT
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, // NOLINT
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, // NOLINT
};

static bool IsAlNum(char c) {
  return ('A' <= c && c <= 'Z')
         || ('a' <= c && c <= 'z')
         || ('0' <= c && c <= '9')
         || (c == '-')
         || (c == '_')
         || (c == '.')
         || (c == '~');
}

const std::string HelpMethos::URLEncode(const std::string &str) {
  std::string res;
  const char *pdata = str.c_str();
  std::size_t str_size = str.size();
  for (std::size_t i = 0; i < str_size; i++) {
    unsigned char ch = pdata[i];
    if (IsAlNum(pdata[i])) {
      res.push_back(pdata[i]);
    } else if (ch >= 128) {
      res.push_back('%');
      res.push_back(TO_HEX((ch >> 4) & 0xF));
      res.push_back(TO_HEX((ch)& 0xF));
    } else {
      res.push_back('%');
      res.push_back(TO_HEX((ch >> 4) & 0xF));
      res.push_back(TO_HEX((ch)& 0xF));
    }
  }
  return res;
}

const std::string HelpMethos::HttpMethodToStr(HttpMethod hm) {
  static const char HTTP_GET_STR[] = "GET";
  static const char HTTP_POST_STR[] = "POST";
  switch (hm) {
  case vzsdk::HTTP_GET:
    return HTTP_GET_STR;
    break;
  case vzsdk::HTTP_POST:
    return HTTP_POST_STR;
    break;
  default:
    break;
  }
  return HTTP_GET_STR;
}

void *memxor(void * dest, const void * src, size_t n) {
  char const *s = (const char *)src;
  char *d = reinterpret_cast<char *>(dest);

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}

#define IPAD 0X36
#define OPAD 0X5C

void HelpMethos::HmacSha1(const uint8 *key, std::size_t key_size,
                          const uint8 *data, std::size_t data_size,
                          uint8 *result) {
  SHA1_CTX inner;
  SHA1_CTX outer;
  unsigned char key_buf[64] = {0};
  char optkeybuf[20];
  char block[64];
  char innerhash[20];

  /* Reduce the key's size, so that it becomes <= 64 bytes large.  */

  if (key_size > 64) {
    SHA1_CTX keyhash;

    SHA1Init(&keyhash);
    SHA1Update(&keyhash, (const unsigned char *)key, key_size);
    SHA1Final(&keyhash, (unsigned char *)optkeybuf);

    memcpy(key_buf, optkeybuf, 20);
    key_size = 20;
  } else {
    memcpy(key_buf, key, key_size);
  }

  /* Compute INNERHASH from KEY and IN.  */

  SHA1Init(&inner);

  memset(block, IPAD, sizeof(block));
  memxor(block, key_buf, key_size);

  SHA1Update(&inner, (const unsigned char *)block, 64);
  SHA1Update(&inner, (const unsigned char *)data, data_size);

  SHA1Final(&inner, (unsigned char *)innerhash);

  /* Compute result from KEY and INNERHASH.  */

  SHA1Init(&outer);

  memset(block, OPAD, sizeof(block));
  memxor(block, key_buf, key_size);

  SHA1Update(&outer, (const unsigned char *)block, 64);
  SHA1Update(&outer, (const unsigned char *)innerhash, 20);

  SHA1Final(&outer, (unsigned char *)result);
}

void HelpMethos::HmacSha1ToBase64(const std::string &key,
                                  const std::string &data,
                                  std::string &result) {
  unsigned char digest_buffer[SHA1_DIGEST_SIZE];
  HmacSha1((const unsigned char *)key.c_str(), key.size(),
           (const unsigned char *)data.c_str(), data.size(), digest_buffer);

  Base64::EncodeFromArray(digest_buffer, SHA1_DIGEST_SIZE, &result);
}

}  // namespace vzsdk
