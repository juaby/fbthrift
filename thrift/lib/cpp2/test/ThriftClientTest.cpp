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

#include <folly/fibers/Fiber.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/portability/GTest.h>
#include <thrift/lib/cpp2/test/gen-cpp2/TestService.h>
#include <thrift/lib/cpp2/util/ScopedServerInterfaceThread.h>
#include <chrono>
#include <condition_variable>

using namespace std;
using namespace std::chrono;
using namespace folly;
using namespace apache::thrift;
using namespace apache::thrift::test;
using namespace apache::thrift::transport;

class ThriftClientTest : public testing::Test {};

TEST_F(ThriftClientTest, FutureCapturesChannel) {
  class Handler : public TestServiceSvIf {
  public:
    Future<unique_ptr<string>> future_sendResponse(int64_t size) override {
      return makeFuture(make_unique<string>(to<string>(size)));
    }
  };

  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);
  auto fut = client->future_sendResponse(12);
  // To prove that even if the client is gone, the channel is captured:
  client = nullptr;
  auto ret = fut.waitVia(&eb).getTry();

  EXPECT_TRUE(ret.hasValue());
  EXPECT_EQ("12", ret.value());
}

TEST_F(ThriftClientTest, SemiFutureCapturesChannel) {
  class Handler : public TestServiceSvIf {
   public:
    void sendResponse(std::string& _return, int64_t size) override {
      _return = to<string>(size);
    }
  };

  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);
  auto fut = client->semifuture_sendResponse(15).via(&eb).waitVia(&eb);

  // To prove that even if the client is gone, the channel is captured:
  client = nullptr;

  EXPECT_EQ("15", fut.value());
}

TEST_F(ThriftClientTest, FutureCapturesChannelOneway) {
  //  Generated SvIf handler methods throw. We check Try::hasValue().
  //  So this is a sanity check that the method is oneway.
  auto handler = make_shared<TestServiceSvIf>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);
  auto fut = client->future_noResponse(12);
  // To prove that even if the client is gone, the channel is captured:
  client = nullptr;
  auto ret = fut.waitVia(&eb).getTry();

  EXPECT_TRUE(ret.hasValue());
}

TEST_F(ThriftClientTest, SemiFutureCapturesChannelOneway) {
  // Ditto previous test but with the SemiFuture<T> methods
  auto handler = make_shared<TestServiceSvIf>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);
  auto fut = client->semifuture_noResponse(12).via(&eb).waitVia(&eb);
  // To prove that even if the client is gone, the channel is captured:
  client = nullptr;

  EXPECT_TRUE(fut.hasValue());
}

TEST_F(ThriftClientTest, SyncRpcOptionsTimeout) {
  class DelayHandler : public TestServiceSvIf {
  public:
    DelayHandler(milliseconds delay) : delay_(delay) {}
    void async_eb_eventBaseAsync(
        unique_ptr<HandlerCallback<unique_ptr<string>>> cb) override {
      auto eb = cb->getEventBase();
      eb->runAfterDelay([cb = move(cb)] {
        cb->result("hello world");
      }, delay_.count());
    }
  private:
    milliseconds delay_;
  };

  //  rpcTimeout << handlerDelay << channelTimeout.
  constexpr auto handlerDelay = milliseconds(10);
  constexpr auto channelTimeout = duration_cast<milliseconds>(seconds(10));
  constexpr auto rpcTimeout = milliseconds(1);

  //  Handler has medium 10ms delay.
  auto handler = make_shared<DelayHandler>(handlerDelay);
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);
  auto channel = dynamic_cast<HeaderClientChannel*>(client->getChannel());
  ASSERT_NE(nullptr, channel);
  channel->setTimeout(channelTimeout.count());

  auto start = steady_clock::now();
  try {
    RpcOptions options;
    options.setTimeout(rpcTimeout);
    std::string response;
    client->sync_eventBaseAsync(options, response);
    ADD_FAILURE() << "should have timed out";
  } catch (const TTransportException& e) {
    auto expected = TTransportException::TIMED_OUT;
    EXPECT_EQ(expected, e.getType());
  }

  auto dur = steady_clock::now() - start;
  EXPECT_EQ(channelTimeout.count(), channel->getTimeout());
  EXPECT_GE(dur, rpcTimeout);
  EXPECT_LT(dur, channelTimeout);
}

TEST_F(ThriftClientTest, SyncCallRequestResponse) {
  class Handler : public TestServiceSvIf {
   public:
    void sendResponse(std::string& _return, int64_t size) override {
      _return = to<string>(size);
    }
  };
  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);

  auto doSyncRPC = [&]() {
    std::string res;
    client->sync_sendResponse(res, 123);
    EXPECT_EQ(res, "123");
  };

  // First test from evbase thread
  doSyncRPC();

  // Now try from fibers
  auto doSomeFibers = [&](EventBase& eb) {
    folly::fibers::Baton b1, b2, b3, b4;
    auto& fm = folly::fibers::getFiberManager(eb);
    fm.addTask([&]() {
      b3.wait();
      b4.wait();
      doSyncRPC();
      eb.terminateLoopSoon();
    });
    fm.addTask([&]() {
      b1.wait();
      doSyncRPC();
      b3.post();
    });
    fm.addTask([&]() {
      b2.wait();
      doSyncRPC();
      b4.post();
    });
    fm.addTask([&]() {
      doSyncRPC();
      b1.post();
    });
    fm.addTask([&]() {
      doSyncRPC();
      b2.post();
    });
    eb.loop();
  };
  doSomeFibers(eb);
}

TEST_F(ThriftClientTest, SyncCallOneWay) {
  class Handler : public TestServiceSvIf {
   public:
    void noResponse(int64_t) override {
      std::lock_guard<std::mutex> l(lock_);
      ++numCalls_;
      condvar_.notify_all();
    }
    int32_t numCalls() const {
      std::lock_guard<std::mutex> l(lock_);
      return numCalls_;
    }
    void waitUntilNumCalls(int32_t goal) {
      std::unique_lock<std::mutex> l(lock_);
      auto deadline = std::chrono::system_clock::now() + 200ms;
      condvar_.wait_until(l, deadline, [&] { return numCalls_ == goal; });
      EXPECT_EQ(numCalls_, goal);
    }

   private:
    mutable std::mutex lock_;
    std::condition_variable condvar_;
    int32_t numCalls_{0};
  };

  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);

  auto doSyncRPC = [&]() { client->sync_noResponse(123); };

  // First test from evbase thread
  doSyncRPC();

  // Now try from fibers
  folly::fibers::Baton b1, b2, b3, b4;
  auto& fm = folly::fibers::getFiberManager(eb);
  fm.addTask([&]() {
    b3.wait();
    b4.wait();
    doSyncRPC();
    eb.terminateLoopSoon();
  });
  fm.addTask([&]() {
    b1.wait();
    doSyncRPC();
    b3.post();
  });
  fm.addTask([&]() {
    b2.wait();
    doSyncRPC();
    b4.post();
  });
  fm.addTask([&]() {
    doSyncRPC();
    b1.post();
  });
  fm.addTask([&]() {
    doSyncRPC();
    b2.post();
  });
  eb.loop();

  handler->waitUntilNumCalls(6);
}

TEST_F(ThriftClientTest, FutureCallRequestResponse) {
  class Handler : public TestServiceSvIf {
   public:
    Future<unique_ptr<string>> future_sendResponse(int64_t size) override {
      return makeFuture(make_unique<string>(to<string>(size)));
    }
  };
  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);

  auto doFutureSyncRPC = [&]() {
    std::string res;
    auto f = client->future_sendResponse(123);
    if (!folly::fibers::onFiber()) {
      while (!f.isReady()) {
        eb.drive();
      }
    }
    auto r = f.wait().getTry();
    ASSERT_TRUE(r.hasValue());
    res = r.value();
    EXPECT_EQ(res, "123");
  };

  // First test from evbase thread
  doFutureSyncRPC();

  // Now try from fibers
  auto doSomeFibers = [&](EventBase& eb) {
    folly::fibers::Baton b1, b2, b3, b4;
    auto& fm = folly::fibers::getFiberManager(eb);
    fm.addTask([&]() {
      b3.wait();
      b4.wait();
      doFutureSyncRPC();
      eb.terminateLoopSoon();
    });
    fm.addTask([&]() {
      b1.wait();
      doFutureSyncRPC();
      b3.post();
    });
    fm.addTask([&]() {
      b2.wait();
      doFutureSyncRPC();
      b4.post();
    });
    fm.addTask([&]() {
      doFutureSyncRPC();
      b1.post();
    });
    fm.addTask([&]() {
      doFutureSyncRPC();
      b2.post();
    });
    eb.loop();
  };
  doSomeFibers(eb);
}

TEST_F(ThriftClientTest, FutureCallOneWay) {
  class Handler : public TestServiceSvIf {
   public:
    void noResponse(int64_t) override {
      std::lock_guard<std::mutex> l(lock_);
      ++numCalls_;
      condvar_.notify_all();
    }
    int32_t numCalls() const {
      std::lock_guard<std::mutex> l(lock_);
      return numCalls_;
    }
    void waitUntilNumCalls(int32_t goal) {
      std::unique_lock<std::mutex> l(lock_);
      auto deadline = std::chrono::system_clock::now() + 200ms;
      condvar_.wait_until(l, deadline, [&] { return numCalls_ == goal; });
      EXPECT_EQ(numCalls_, goal);
    }

   private:
    mutable std::mutex lock_;
    std::condition_variable condvar_;
    int32_t numCalls_{0};
  };
  auto handler = make_shared<Handler>();
  ScopedServerInterfaceThread runner(handler);

  EventBase eb;
  auto client = runner.newClient<TestServiceAsyncClient>(&eb);

  auto doFutureSyncRPC = [&]() {
    auto f = client->future_noResponse(123);
    if (!folly::fibers::onFiber()) {
      while (!f.isReady()) {
        eb.drive();
      }
    }
    auto r = std::move(f).wait().getTry();
    EXPECT_TRUE(r.hasValue());
  };

  // First test from evbase thread
  doFutureSyncRPC();

  // Now try from fibers
  folly::fibers::Baton b1, b2, b3, b4;
  auto& fm = folly::fibers::getFiberManager(eb);
  fm.addTask([&]() {
    b3.wait();
    b4.wait();
    doFutureSyncRPC();
    eb.terminateLoopSoon();
  });
  fm.addTask([&]() {
    b1.wait();
    doFutureSyncRPC();
    b3.post();
  });
  fm.addTask([&]() {
    b2.wait();
    doFutureSyncRPC();
    b4.post();
  });
  fm.addTask([&]() {
    doFutureSyncRPC();
    b1.post();
  });
  fm.addTask([&]() {
    doFutureSyncRPC();
    b2.post();
  });
  eb.loop();

  handler->waitUntilNumCalls(6);
}
