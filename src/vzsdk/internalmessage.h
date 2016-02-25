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

#ifndef SRC_HSHA_INTERNAL_MESSAGE_H_
#define SRC_HSHA_INTERNAL_MESSAGE_H_

#include "base/noncopyable.h"
#include "base/socketaddress.h"
#include "event/messagequeue.h"
#include "json/json.h"

namespace vzsdk {

enum ReqType {
};

enum StanzaType {
  REQ_CONNECT_SERVER,
  REQ_DISCONNECT_SERVER,
  REQ_SEND_REQUESTION,
  REQ_ADD_PUSH_TASK,
  REQ_REMOVE_TASK_EVENT,

  REQ_RES_BREAK = 0XFF,

  RES_DISCONNECTED_EVENT_FAILURE,
  RES_DISCONNECTED_EVENT_SUCCEED,
  RES_SESSION_NOT_FOUND,
  RES_PUSH_SUCCEED,
  RES_CONNECTED_EVENT,
  RES_STANZA_EVENT,
};

const uint32 NULL_SESSION_ID = 0;

class Stanza : public MessageData {
 public:
  Stanza(uint32 stanza_type, uint32 session_id = NULL_SESSION_ID)
    : stanza_type_(stanza_type),
      session_id_(session_id) {
  }
  uint32 stanza_type() const {
    return stanza_type_;
  }
  void set_stanza_type(uint32 stanza_type) {
    stanza_type_ = stanza_type;
  }
  uint32 session_id() const {
    return session_id_;
  }
  void set_session_id(uint32 session_id) {
    session_id_ = session_id;
  }
 private:
  uint32 stanza_type_;
  uint32 session_id_;
};

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
class ReqConnectData : public Stanza {
 public:
  ReqConnectData(SocketAddress &address);
  SocketAddress &address() {
    return address_;
  }
 private:
  SocketAddress address_;
};

//------------------------------------------------------------------------------
class ReqDisconnectData : public Stanza {
 public:
  ReqDisconnectData(uint32 session_id);
};

//------------------------------------------------------------------------------
class RequestData : public Stanza {
 public:
  RequestData(uint32 session_id,
              const Json::Value &req_json,
              bool is_push = false);
  const Json::Value &req_json() {
    return req_json_;
  }
  bool is_push() {
    return is_push_;
  }
 private:
  Json::Value req_json_;
  bool is_push_;
};

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
class ResponseData : public Stanza {
 public:
  ResponseData(uint32 session_id,
               const Json::Value &res_json,
               const std::string &res_data);
  const Json::Value res_json() const {
    return res_json_;
  }
  const std::string res_data() const {
    return res_data_;
  }
 private:
  Json::Value res_json_;
  std::string res_data_;
};
}

#endif // SRC_HSHA_INTERNAL_MESSAGE_H_