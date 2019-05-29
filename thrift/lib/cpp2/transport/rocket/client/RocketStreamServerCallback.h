/*
 * Copyright 2019-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
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

#include <folly/ExceptionWrapper.h>
#include <folly/io/IOBuf.h>
#include <folly/io/async/Request.h>

#include <thrift/lib/cpp2/async/StreamCallbacks.h>
#include <thrift/lib/cpp2/transport/rocket/Types.h>
#include <thrift/lib/thrift/gen-cpp2/RpcMetadata_types.h>

namespace apache {
namespace thrift {

namespace rocket {
class RocketClient;
} // namespace rocket

class RocketStreamServerCallback : public StreamServerCallback {
 public:
  RocketStreamServerCallback(
      rocket::StreamId streamId,
      rocket::RocketClient& client,
      StreamClientCallback& clientCallback)
      : client_(client), clientCallback_(clientCallback), streamId_(streamId) {}

  void onStreamRequestN(uint64_t tokens) override;
  void onStreamCancel() override;

  StreamClientCallback& getClientCallback() const {
    return clientCallback_;
  }

 private:
  rocket::RocketClient& client_;
  StreamClientCallback& clientCallback_;
  rocket::StreamId streamId_;
};

} // namespace thrift
} // namespace apache