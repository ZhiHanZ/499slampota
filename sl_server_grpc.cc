#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
#include "sl_backend.cc"
#include "protos/chirpsl.grpc.pb.h"
#else
#include "chirpsl.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#endif

// Service Layer Server part of grpc that makes calls to the true sl_backend
class ServiceLayerServer final : public chirp::ServiceLayer::Service {
  public:
    grpc::Status RegisterUser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) override {
      std::string username = request.get_username();
      *reply = slbe.Register(username);
      return grpc::Status::OK;
    }

    grpc::Status Chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) override {
      std::string username = request.username();
      std::string text = request.text();
      std::string parent_id = request.parent_id();
      slbe.Chirp(chirp_id, serialized_chirp);
      key_counter_++;
      return grpc::Status::OK;
    }

    grpc::Status Follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) override {
      // TODO: figure out what goes in here???
      std::string username = request.username();
      std::string to_follow = request.to_follow();
      slbe.Follow(username, to_follow);
      return Status::OK;
    }
    grpc::Status Read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) override {
      // TODO: figure out what goes in here???
      std::string chirp_id = request.chirp_id();
      *reply = slbe.Read(chirp_id);
      return grpc::Status::OK;
    }
    grpc::Status Monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply) override {
      // TODO: figure out what goes in here???
      std::string username = request.username();
      *reply = slbe.Monitor(username);
      return grpc::Status::OK;
    }
  private:
    ServiceLayerBackEnd slbe;
    int key_counter_ = 0;
};

void RunServer() {
  std::string server_address("0.0.0.0:50002");
  ServiceLayerServer service_layer;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service_layer);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
} 