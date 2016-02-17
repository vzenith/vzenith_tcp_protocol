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

#ifndef SRC_HSHA_QUEUE_LAYER_H_
#define SRC_HSHA_QUEUE_LAYER_H_

#include "base/noncopyable.h"
#include "event/thread.h"
#include "base/scoped_ptr.h"
#include "vzsdk/sessionmanager.h"
#include "vzsdk/task.h"

namespace vzsdk {
class QueueLayer : public noncopyable,
  public sigslot::has_slots<>,
  public MessageHandler  {
 public:
  QueueLayer();
  virtual ~QueueLayer();
  virtual void OnMessage(Message *msg);
  bool Start();
  bool Stop();
  Thread *queue_thread() {
    return queue_thread_.get();
  }
  void Post(uint32 id, MessageData::Ptr pdata);
  void Post(Task::Ptr task);
 private:
  void OnReqMessage(Message *msg);
  void OnResMessage(Message *msg);

  void OnConnectedEvent(uint32 task_id, Message *msg);

  bool AddTask(Task::Ptr task);
  bool RemoveTask(uint32 task_id);
  Task::Ptr FindTask(uint32 task_id);
 private:
  scoped_ptr<Thread> queue_thread_;
  scoped_ptr<SessionManager> session_manager_;
  std::map<uint32, Task::Ptr> tasks_;
};
}

#endif // SRC_HSHA_QUEUE_LAYER_H_