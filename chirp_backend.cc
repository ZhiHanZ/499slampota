#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD

#else

#endif

// Logic and data behind the server's behavior.
class ChirpServiceImpl final : public chirp::KeyValueStore::Service {

  Status Put(ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override {
    // TODO: call Put in the KeyValueDataStore and then place output into reply
  }

  Status Get(ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) override {
    // TODO: call Get in the KeyValueDataStore and then place output into reply
  }

  Status DeleteKey(ServerContext* context, const chirp::DelteKeyRequest* request, chirp::DeleteKeyReply* reply) override {
    // TODO: call DeleteKey in the KeyValueDataStore and then place output into reply
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