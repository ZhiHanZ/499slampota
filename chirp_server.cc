 
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
// #include "examples/protos/chirp.grpc.pb.h"
#else
// #include "chirp.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chirp::Register;
using chirp:Chirp;
using chirp::Follow;
using chirp::Read;
using chirp::Monitor;

// Logic and data behind the server's behavior.
class ChirpServiceImpl final : public Chirp::Service {
  Status Register(ServerContext* context, const RegisterRequest* request,
                  RegisterReply* reply) override {
    // what is override???
    // do I change Status Register return type to Register Reply return type???
    // where do all these types - like RegisterRequest/Replay get declared???
  }

  Status Chirp(ServerContext* context, const ChirpRequest* request,
                       ChirpReply* reply) override {
  }

  Status Follow(ServerContext* context, const FollowRequest* request,
                       FollowReply* reply) override {

  }

  Status Read(ServerContext* context, const ReadRequest* request,
                  ReadReply* reply) override {
    
  }

  Status Monitor(ServerContext* context, const MonitorRequest* request,
                  MonitorReply* reply) override {
    
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
