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

#ifndef SRC_VZSDK_VZSDKPUSHMANAGER_H_
#define SRC_VZSDK_VZSDKPUSHMANAGER_H_

#include "base/noncopyable.h"
#include "vzsdk/queuelayer.h"
#include "vzsdk/task.h"
#include "vzsdk/vzsdkpushhandle.h"

namespace vzsdk {
class PushManagerTask : public Task {
  public:
    PushManagerTask(QueueLayer *queue_layer, Thread *push_thread);
    virtual ~PushManagerTask();
    virtual uint32 message_type() {
        return REQ_ADD_PUSH_TASK;
    }
    virtual void OnMessage(Message *msg);
    virtual Message::Ptr SyncProcessTask();
    virtual bool HandleMessage(Message *msg);
    void AddPushHandle(PushHandle::Ptr handle);
    void RemovePushHandle(PushHandle::Ptr handle);
  private:
    bool HandleResponse(Message *msg);
    bool HandleChangeConn(Message *msg);
    void ProcessPushEvent(Message *msg);
  private:
    static const uint32 PUSH_EVENT_STANZA = 1;
    static const uint32 ADD_PUSH_HANDLE = 2;
    static const uint32 REMOVE_PUSH_HANDLE = 3;
    bool is_register_;
    typedef std::set<std::string> PushHandleKeys;
    typedef std::vector<PushHandle::Ptr> PushHandles;
    PushHandleKeys push_handle_keys_;
    PushHandles push_handles_;
    mutable CriticalSection crit_;
};
}

#endif // SRC_VZSDK_VZSDKPUSHMANAGER_H_