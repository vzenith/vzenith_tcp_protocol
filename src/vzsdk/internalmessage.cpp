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

#include "vzsdk/internalmessage.h"

namespace vzsdk {
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
ReqConnectData::ReqConnectData(SocketAddress &address)
    : address_(address),
      Stanza(REQ_CONNECT_SERVER) {
}

ReqConnectData::ReqConnectData(const ReqConnectData& _req_connect_data)
    : Stanza(_req_connect_data)
    , address_(_req_connect_data.address_) {
}

//------------------------------------------------------------------------------
ReqDisconnectData::ReqDisconnectData(uint32 session_id)
    : Stanza(REQ_DISCONNECT_SERVER, session_id) {
}

//------------------------------------------------------------------------------
RequestData::RequestData(uint32 session_id,
                         const Json::Value &req_json,
                         bool is_push)
    : Stanza(REQ_SEND_REQUESTION, session_id),
      req_json_(req_json),
      is_push_(is_push) {
}

//------------------------------------------------------------------------------
ResponseData::ResponseData(uint32 session_id,
                           const Json::Value &res_json,
                           const std::string &res_data)
    : Stanza(RES_STANZA_EVENT, session_id),
      res_json_(res_json),
      res_data_(res_data) {
}

Stanza::Stanza(const Stanza& _stanze) {
    this->session_id_ = _stanze.session_id_;
    this->stanza_type_ = _stanze.stanza_type_;
}

}
