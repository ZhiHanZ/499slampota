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

// Service Layer Server part of grpc that makes calls to the true sl_backend
class ServiceLayerServer final : public chirp::ServiceLayer::Service {
  public:
    Status RegisterUser(ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) override {
      // TODO: figure out what goes in here???
      // i think i have to pull out the individual data members in the request 
      // and then use those as the parameter when I call the slbe's methods
      return Status::OK;
    }

    Status Chirp(ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) override {
      // TODO: figure out what goes in here???
      return Status::OK;
    }

    Status Follow(ServerContext* context, const chirp:FollowRequest* request, chirp::FollowReply* reply) override {
      // TODO: figure out what goes in here???
      return Status::OK;
    }
    Status Read(ServerContext* context, const chirp:ReadRequest* request, chirp::ReadReply* reply) override {
      // TODO: figure out what goes in here???
      return Status::OK;
    }
    Status Monitor(ServerContext* context, const chirp:MonitorRequest* request, chirp::MonitorReply* reply) override {
      // TODO: figure out what goes in here???
      return Status::OK;
    }
  private:
    ServiceLayerBackEnd slbe;
};

void RunServer() {
  std::string server_address("0.0.0.0:50002");
  ServiceLayerServer service_layer;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service_layer);
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