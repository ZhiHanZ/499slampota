#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD

#else

#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Logic and data behind the server's behavior.
class ChirpServiceImpl final : public chirp::ServerLayer::Service {
  Status Register(ServerContext* context, const chirp::RegisterRequest* request,
                  chirp::RegisterReply* reply) override {
    // what is override???
    // do I change Status Register return type to Register Reply return type???
    // where do all these types - like RegisterRequest/Replay get declared???
  }

  Status Chirp(ServerContext* context, const chirp::ChirpRequest* request,
                       chirp::ChirpReply* reply) override {
  }

  Status Follow(ServerContext* context, const chirp::FollowRequest* request,
                       chirp::FollowReply* reply) override {

  }

  Status Read(ServerContext* context, const chirp::ReadRequest* request,
                  chirp::ReadReply* reply) override {
    
  }

  Status Monitor(ServerContext* context, const chirp::MonitorRequest* request,
                  chirp::MonitorReply* reply) override {
    
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50002");
  ChirpServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}