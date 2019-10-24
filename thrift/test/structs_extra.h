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

#include <folly/small_vector.h>
#include <folly/sorted_vector_types.h>

namespace apache {
namespace thrift {
namespace test {

template <class T>
struct WrappedType {
  T raw;

  T& rawAccessor() {
    return raw;
  }

  const T& rawAccessor() const {
    return raw;
  }
};

template <typename Value>
using SmallSortedVectorSet = folly::sorted_vector_set<
    Value,
    std::less<Value>,
    std::allocator<Value>,
    void,
    folly::small_vector<Value, 1>>;

template <typename Key, typename Mapped>
using SmallSortedVectorMap = folly::sorted_vector_map<
    Key,
    Mapped,
    std::less<Key>,
    std::allocator<std::pair<Key, Mapped>>,
    void,
    folly::small_vector<std::pair<Key, Mapped>, 1>>;

} // namespace test
} // namespace thrift
} // namespace apache
