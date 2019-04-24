/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#pragma once

#include "thrift/compiler/test/fixtures/basic-stack-arguments/gen-cpp2/MyService.h"

#include <folly/io/IOBuf.h>
#include <folly/io/IOBufQueue.h>
#include <thrift/lib/cpp/TApplicationException.h>
#include <thrift/lib/cpp/transport/THeader.h>
#include <thrift/lib/cpp2/GeneratedCodeHelper.h>
#include <thrift/lib/cpp2/GeneratedSerializationCodeHelper.h>
#include <thrift/lib/cpp2/gen/service_tcc.h>
#include <thrift/lib/cpp2/server/Cpp2ConnContext.h>

namespace cpp2 {
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, apache::thrift::protocol::T_I64, int64_t*>> MyService_hasDataById_pargs;
typedef apache::thrift::ThriftPresult<true, apache::thrift::FieldData<0, apache::thrift::protocol::T_BOOL, bool*>> MyService_hasDataById_presult;
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, apache::thrift::protocol::T_I64, int64_t*>> MyService_getDataById_pargs;
typedef apache::thrift::ThriftPresult<true, apache::thrift::FieldData<0, apache::thrift::protocol::T_STRING, std::string*>> MyService_getDataById_presult;
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, apache::thrift::protocol::T_I64, int64_t*>, apache::thrift::FieldData<2, apache::thrift::protocol::T_STRING, std::string*>> MyService_putDataById_pargs;
typedef apache::thrift::ThriftPresult<true> MyService_putDataById_presult;
typedef apache::thrift::ThriftPresult<false, apache::thrift::FieldData<1, apache::thrift::protocol::T_I64, int64_t*>, apache::thrift::FieldData<2, apache::thrift::protocol::T_STRING, std::string*>> MyService_lobDataById_pargs;
template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::_processInThread_hasDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  auto pri = iface_->getRequestPriority(ctx, apache::thrift::concurrency::NORMAL);
  processInThread<ProtocolIn_, ProtocolOut_>(std::move(req), std::move(buf),std::move(iprot), ctx, eb, tm, pri, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, &MyServiceAsyncProcessor::process_hasDataById<ProtocolIn_, ProtocolOut_>, this);
}
template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::process_hasDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot,apache::thrift::Cpp2RequestContext* ctx,folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  // make sure getConnectionContext is null
  // so async calls don't accidentally use it
  iface_->setConnectionContext(nullptr);
  MyService_hasDataById_pargs args;
  int64_t uarg_id{0};
  args.get<0>().value = &uarg_id;
  std::unique_ptr<apache::thrift::ContextStack> ctxStack(this->getContextStack(apache::thrift::ContextStack::NameWrapper::makeFromStatic(this->getServiceName()), apache::thrift::ContextStack::NameWrapper::makeFromStatic("MyService.hasDataById"), ctx));
  try {
    deserializeRequest(args, buf.get(), iprot.get(), ctxStack.get());
  }
  catch (const std::exception& ex) {
    apache::thrift::detail::ap::process_handle_exn_deserialization<ProtocolOut_>(
        ex, std::move(req), ctx, eb, "hasDataById");
    return;
  }
  req->setStartedProcessing();
  auto callback = std::make_unique<apache::thrift::HandlerCallback<bool>>(std::move(req), std::move(ctxStack), return_hasDataById<ProtocolIn_,ProtocolOut_>, throw_wrapped_hasDataById<ProtocolIn_, ProtocolOut_>, ctx->getProtoSeqId(), eb, tm, ctx);
  if (!callback->isRequestActive()) {
    callback.release()->deleteInThread();
    return;
  }
  ctx->setStartedProcessing();
  iface_->async_tm_hasDataById(std::move(callback), args.get<0>().ref());
}

template <class ProtocolIn_, class ProtocolOut_>
folly::IOBufQueue MyServiceAsyncProcessor::return_hasDataById(int32_t protoSeqId, apache::thrift::ContextStack* ctx, bool const& _return) {
  ProtocolOut_ prot;
  MyService_hasDataById_presult result;
  result.get<0>().value = const_cast<bool*>(&_return);
  result.setIsSet(0, true);
  return serializeResponse("hasDataById", &prot, protoSeqId, ctx, result);
}

template <class ProtocolIn_, class ProtocolOut_>
void MyServiceAsyncProcessor::throw_wrapped_hasDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req,int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx) {
  if (!ew) {
    return;
  }
  {
    (void)protoSeqId;
    apache::thrift::detail::ap::process_throw_wrapped_handler_error<ProtocolOut_>(
        ew, std::move(req), reqCtx, ctx, "hasDataById");
    return;
  }
}

template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::_processInThread_getDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  auto pri = iface_->getRequestPriority(ctx, apache::thrift::concurrency::NORMAL);
  processInThread<ProtocolIn_, ProtocolOut_>(std::move(req), std::move(buf),std::move(iprot), ctx, eb, tm, pri, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, &MyServiceAsyncProcessor::process_getDataById<ProtocolIn_, ProtocolOut_>, this);
}
template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::process_getDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot,apache::thrift::Cpp2RequestContext* ctx,folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  // make sure getConnectionContext is null
  // so async calls don't accidentally use it
  iface_->setConnectionContext(nullptr);
  MyService_getDataById_pargs args;
  int64_t uarg_id{0};
  args.get<0>().value = &uarg_id;
  std::unique_ptr<apache::thrift::ContextStack> ctxStack(this->getContextStack(apache::thrift::ContextStack::NameWrapper::makeFromStatic(this->getServiceName()), apache::thrift::ContextStack::NameWrapper::makeFromStatic("MyService.getDataById"), ctx));
  try {
    deserializeRequest(args, buf.get(), iprot.get(), ctxStack.get());
  }
  catch (const std::exception& ex) {
    apache::thrift::detail::ap::process_handle_exn_deserialization<ProtocolOut_>(
        ex, std::move(req), ctx, eb, "getDataById");
    return;
  }
  req->setStartedProcessing();
  auto callback = std::make_unique<apache::thrift::HandlerCallback<std::string>>(std::move(req), std::move(ctxStack), return_getDataById<ProtocolIn_,ProtocolOut_>, throw_wrapped_getDataById<ProtocolIn_, ProtocolOut_>, ctx->getProtoSeqId(), eb, tm, ctx);
  if (!callback->isRequestActive()) {
    callback.release()->deleteInThread();
    return;
  }
  ctx->setStartedProcessing();
  iface_->async_tm_getDataById(std::move(callback), args.get<0>().ref());
}

template <class ProtocolIn_, class ProtocolOut_>
folly::IOBufQueue MyServiceAsyncProcessor::return_getDataById(int32_t protoSeqId, apache::thrift::ContextStack* ctx, std::string const& _return) {
  ProtocolOut_ prot;
  MyService_getDataById_presult result;
  result.get<0>().value = const_cast<std::string*>(&_return);
  result.setIsSet(0, true);
  return serializeResponse("getDataById", &prot, protoSeqId, ctx, result);
}

template <class ProtocolIn_, class ProtocolOut_>
void MyServiceAsyncProcessor::throw_wrapped_getDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req,int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx) {
  if (!ew) {
    return;
  }
  {
    (void)protoSeqId;
    apache::thrift::detail::ap::process_throw_wrapped_handler_error<ProtocolOut_>(
        ew, std::move(req), reqCtx, ctx, "getDataById");
    return;
  }
}

template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::_processInThread_putDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  auto pri = iface_->getRequestPriority(ctx, apache::thrift::concurrency::NORMAL);
  processInThread<ProtocolIn_, ProtocolOut_>(std::move(req), std::move(buf),std::move(iprot), ctx, eb, tm, pri, apache::thrift::RpcKind::SINGLE_REQUEST_SINGLE_RESPONSE, &MyServiceAsyncProcessor::process_putDataById<ProtocolIn_, ProtocolOut_>, this);
}
template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::process_putDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot,apache::thrift::Cpp2RequestContext* ctx,folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  // make sure getConnectionContext is null
  // so async calls don't accidentally use it
  iface_->setConnectionContext(nullptr);
  MyService_putDataById_pargs args;
  int64_t uarg_id{0};
  args.get<0>().value = &uarg_id;
  std::string uarg_data;
  args.get<1>().value = &uarg_data;
  std::unique_ptr<apache::thrift::ContextStack> ctxStack(this->getContextStack(apache::thrift::ContextStack::NameWrapper::makeFromStatic(this->getServiceName()), apache::thrift::ContextStack::NameWrapper::makeFromStatic("MyService.putDataById"), ctx));
  try {
    deserializeRequest(args, buf.get(), iprot.get(), ctxStack.get());
  }
  catch (const std::exception& ex) {
    apache::thrift::detail::ap::process_handle_exn_deserialization<ProtocolOut_>(
        ex, std::move(req), ctx, eb, "putDataById");
    return;
  }
  req->setStartedProcessing();
  auto callback = std::make_unique<apache::thrift::HandlerCallback<void>>(std::move(req), std::move(ctxStack), return_putDataById<ProtocolIn_,ProtocolOut_>, throw_wrapped_putDataById<ProtocolIn_, ProtocolOut_>, ctx->getProtoSeqId(), eb, tm, ctx);
  if (!callback->isRequestActive()) {
    callback.release()->deleteInThread();
    return;
  }
  ctx->setStartedProcessing();
  iface_->async_tm_putDataById(std::move(callback), args.get<0>().ref(), args.get<1>().ref());
}

template <class ProtocolIn_, class ProtocolOut_>
folly::IOBufQueue MyServiceAsyncProcessor::return_putDataById(int32_t protoSeqId, apache::thrift::ContextStack* ctx) {
  ProtocolOut_ prot;
  MyService_putDataById_presult result;
  return serializeResponse("putDataById", &prot, protoSeqId, ctx, result);
}

template <class ProtocolIn_, class ProtocolOut_>
void MyServiceAsyncProcessor::throw_wrapped_putDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req,int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx) {
  if (!ew) {
    return;
  }
  {
    (void)protoSeqId;
    apache::thrift::detail::ap::process_throw_wrapped_handler_error<ProtocolOut_>(
        ew, std::move(req), reqCtx, ctx, "putDataById");
    return;
  }
}

template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::_processInThread_lobDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  auto pri = iface_->getRequestPriority(ctx, apache::thrift::concurrency::NORMAL);
  processInThread<ProtocolIn_, ProtocolOut_>(std::move(req), std::move(buf),std::move(iprot), ctx, eb, tm, pri, apache::thrift::RpcKind::SINGLE_REQUEST_NO_RESPONSE, &MyServiceAsyncProcessor::process_lobDataById<ProtocolIn_, ProtocolOut_>, this);
}
template <typename ProtocolIn_, typename ProtocolOut_>
void MyServiceAsyncProcessor::process_lobDataById(std::unique_ptr<apache::thrift::ResponseChannelRequest> req, std::unique_ptr<folly::IOBuf> buf, std::unique_ptr<ProtocolIn_> iprot,apache::thrift::Cpp2RequestContext* ctx,folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) {
  // make sure getConnectionContext is null
  // so async calls don't accidentally use it
  iface_->setConnectionContext(nullptr);
  MyService_lobDataById_pargs args;
  int64_t uarg_id{0};
  args.get<0>().value = &uarg_id;
  std::string uarg_data;
  args.get<1>().value = &uarg_data;
  std::unique_ptr<apache::thrift::ContextStack> ctxStack(this->getContextStack(apache::thrift::ContextStack::NameWrapper::makeFromStatic(this->getServiceName()), apache::thrift::ContextStack::NameWrapper::makeFromStatic("MyService.lobDataById"), ctx));
  try {
    deserializeRequest(args, buf.get(), iprot.get(), ctxStack.get());
  }
  catch (const std::exception& ex) {
    LOG(ERROR) << ex.what() << " in function lobDataById";
    return;
  }
  auto callback = std::make_unique<apache::thrift::HandlerCallbackBase>(std::move(req), std::move(ctxStack), nullptr, eb, tm, ctx);
  ctx->setStartedProcessing();
  iface_->async_tm_lobDataById(std::move(callback), args.get<0>().ref(), args.get<1>().ref());
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
