
#include <iostream>
#include <memory>
#include <string>

// #include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
// #include "examples/protos/chirp.grpc.pb.h"
#else
// #include "chirp.grpc.pb.h"
#endif


// Logic and data behind the server's behavior.
class ChirpServiceImpl final : public Chirp::Service {

  PutReply Put(ServerContext* context, const PutRequest* request) override {
    // or should return type be: "Status"???
  }

  GetReply Get(ServerContext* context, const GetRequest* request) override {
  }

  DeleteKeyReply DeleteKey(ServerContext* context, const DelteKeyRequest* request) override {
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50000");
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
