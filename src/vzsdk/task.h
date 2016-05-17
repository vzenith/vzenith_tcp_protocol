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

#ifndef SRC_HSHA_TASK_H_
#define SRC_HSHA_TASK_H_

#include "base/noncopyable.h"
#include "event/thread.h"
#include "base/scoped_ptr.h"
#include "vzsdk/internalmessage.h"
#include "vzsdk/vzlprtcpsdk.h"

namespace vzsdk {

class QueueLayer;

class Task : public MessageHandler,
    public boost::enable_shared_from_this<Task> {
  public:
    typedef boost::shared_ptr<Task> Ptr;
    Task(QueueLayer *queue_layer, uint32 timeout, Thread *task_thread = NULL);
    virtual ~Task();
    virtual void OnMessage(Message *msg);
    virtual MessageData::Ptr message_data() {
        return message_data_;
    }
    virtual uint32 message_type() = 0;
    virtual Message::Ptr SyncProcessTask();
    virtual void PostTask();

    Thread *task_thread() {
        return task_thread_;
    }
    uint32 task_id() const {
        return task_id_;
    }
    virtual bool HandleMessage(Message *msg);
  protected:
    void set_message_data(MessageData *msg_data) {
        message_data_.reset(msg_data);
    }

  protected:
    Message::Ptr WaitTaskDone();
  protected:
    static int unequal_task_id_;
    QueueLayer *queue_layer_;
    uint32 timeout_;
    Thread *task_thread_;
    uint32 task_id_;
    MessageData::Ptr message_data_;
    bool unwrap_thread_;
};

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
class ConnectTask : public Task {
  public:
    ConnectTask(QueueLayer *queue_layer,
                uint32 timeout,
                SocketAddress &address);
    ConnectTask(QueueLayer *queue_layer
                , uint32 timeout
                , const ReqConnectData& _req_connect_data);
    virtual ~ConnectTask();

    virtual uint32 message_type() {
        return REQ_CONNECT_SERVER;
    }
  private:
    ReqConnectData *req_connect_data_;
};

class ReConnectTask : public Task {
  public:
    ReConnectTask(QueueLayer *queue_layer
                  , uint32 timeout
                  , const ReqConnectData& _req_connect_data);
    virtual ~ReConnectTask();

    virtual void OnMessage(Message *msg);
    virtual uint32 message_type() {
        return RES_RECONNECT_EVENT;
    }
  private:
    ReqConnectData *req_reconnect_data_;
};

//------------------------------------------------------------------------------
class DisconnectTask : public Task {
  public:
    DisconnectTask(QueueLayer *queue_layer,
                   uint32 timeout,
                   uint32 session_id);
    virtual ~DisconnectTask();
    virtual uint32 message_type() {
        return REQ_DISCONNECT_SERVER;
    }
    virtual bool HandleMessage(Message *msg);
  private:
    ReqDisconnectData *req_disconnect_data_;
};

//------------------------------------------------------------------------------
class ReqTask : public Task {
  public:
    ReqTask(QueueLayer *queue_layer,
            uint32 timeout,
            uint32 session_id,
            const Json::Value &req_json
            , Thread *task_thread = NULL);
    virtual ~ReqTask();
    virtual uint32 message_type() {
        return REQ_SEND_REQUESTION;
    }
    virtual bool HandleMessage(Message *msg);
  protected:
    virtual bool HandleResponse(Message *msg);
    RequestData *req_data_;
    std::string req_cmd_;
};

//------------------------------------------------------------------------------
class ReqPushTask : public Task {
  public:
    ReqPushTask(QueueLayer *queue_layer,
                uint32 timeout,
                uint32 session_id,
                const Json::Value &req_json);
    virtual ~ReqPushTask();
    virtual void OnMessage(Message *msg);
    virtual uint32 message_type() {
        return REQ_SEND_REQUESTION;
    }
    virtual bool HandleMessage(Message *msg);
  private:
    RequestData *req_data_;
};

//------------------------------------------------------------------------------
class ReqRecordTask : public ReqTask {
  public:
    ReqRecordTask(QueueLayer *queue_layer,
                  uint32 timeout,
                  uint32 session_id,
                  const Json::Value &req_json);
    virtual ~ReqRecordTask();
  protected:
    virtual bool HandleResponse(Message *msg);
};

}

#endif // SRC_HSHA_TASK_H_