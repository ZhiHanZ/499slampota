#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
// TODO: I'm not sure what to put here in place of the proto stuff that hello world uses
// #include "examples/protos/helloworld.grpc.pb.h"
// #else
// #include "helloworld.grpc.pb.h"
#else

#endif

// Key Value Server part of grpc that makes calls to the true kv_backend
class KeyValueServer final : public chirp::KeyValueStore::Service {
  public:
    Status Put(ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override {
      // TODO: find out how to generate random keys
      int key = 0;
      kvbe.Put(key, request);
      return Status::OK;
    }

    Status Get(ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) override {
      *reply = kvbe.Get(request);
      return Status::OK;
    }

    Status DeleteKey(ServerContext* context, const chirp::DelteKeyRequest* request, chirp::DeleteKeyReply* reply) override {
      kvbe.DeleteKey(request);
      return Status::OK;
    }
  private:
    KeyValueBackEnd kvbe;
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