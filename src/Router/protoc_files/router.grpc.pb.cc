// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: router.proto

#include "router.pb.h"
#include "router.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace router {

static const char* RouterService_method_names[] = {
  "/router.RouterService/Router",
};

std::unique_ptr< RouterService::Stub> RouterService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< RouterService::Stub> stub(new RouterService::Stub(channel));
  return stub;
}

RouterService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Router_(RouterService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RouterService::Stub::Router(::grpc::ClientContext* context, const ::router::RouterRequest& request, ::router::LookupResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Router_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::router::LookupResponse>* RouterService::Stub::AsyncRouterRaw(::grpc::ClientContext* context, const ::router::RouterRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::router::LookupResponse>::Create(channel_.get(), cq, rpcmethod_Router_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::router::LookupResponse>* RouterService::Stub::PrepareAsyncRouterRaw(::grpc::ClientContext* context, const ::router::RouterRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::router::LookupResponse>::Create(channel_.get(), cq, rpcmethod_Router_, context, request, false);
}

RouterService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RouterService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RouterService::Service, ::router::RouterRequest, ::router::LookupResponse>(
          std::mem_fn(&RouterService::Service::Router), this)));
}

RouterService::Service::~Service() {
}

::grpc::Status RouterService::Service::Router(::grpc::ServerContext* context, const ::router::RouterRequest* request, ::router::LookupResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace router

