/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>
#include <utility>

#include <glog/logging.h>

#include <folly/Function.h>
#include <folly/io/async/DelayedDestruction.h>

#include <thrift/lib/cpp2/async/ClientChannel.h>
#include <thrift/lib/cpp2/async/RequestChannel.h>

namespace folly {
class EventBase;
class IOBuf;
} // namespace folly

namespace apache {
namespace thrift {

namespace transport {
class THeader;
} // namespace transport

// Simple RequestChannel wrapper that automatically re-creates underlying
// RequestChannel in case request is about to be sent over a bad channel.
class ReconnectingRequestChannel : public RequestChannel {
 public:
  using Impl = ClientChannel;
  using ImplPtr = std::shared_ptr<Impl>;
  using ImplCreator = folly::Function<ImplPtr(folly::EventBase&)>;

  static std::unique_ptr<
      ReconnectingRequestChannel,
      folly::DelayedDestruction::Destructor>
  newChannel(folly::EventBase& evb, ImplCreator implCreator) {
    return {new ReconnectingRequestChannel(evb, std::move(implCreator)), {}};
  }

  void sendRequestResponse(
      RpcOptions& options,
      std::unique_ptr<folly::IOBuf> buf,
      std::shared_ptr<transport::THeader> header,
      RequestClientCallback::Ptr cob) override;

  void sendRequestNoResponse(
      RpcOptions&,
      std::unique_ptr<folly::IOBuf>,
      std::shared_ptr<transport::THeader>,
      RequestClientCallback::Ptr) override {
    LOG(FATAL) << "Not supported";
  }

  void setCloseCallback(CloseCallback*) override {
    LOG(FATAL) << "Not supported";
  }

  folly::EventBase* getEventBase() const override {
    return &evb_;
  }

  uint16_t getProtocolId() override {
    return impl().getProtocolId();
  }

 protected:
  ~ReconnectingRequestChannel() override = default;

 private:
  ReconnectingRequestChannel(folly::EventBase& evb, ImplCreator implCreator)
      : implCreator_(std::move(implCreator)), evb_(evb) {}

  Impl& impl();

  ImplPtr impl_;
  ImplCreator implCreator_;
  folly::EventBase& evb_;
};

} // namespace thrift
} // namespace apache
