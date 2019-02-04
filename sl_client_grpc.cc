#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
// TODO: I'm not sure what to put here in place of the proto stuff that hello world uses
// #include "examples/protos/helloworld.grpc.pb.h"
// #else
// #include "helloworld.grpc.pb.h"
#endif

// Service Layer Client Class that calls sl_server_grpc functions
class ServiceLayerClient : public ServiceLayerInterface {
 public:
  ServiceLayerClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(chirp::ServiceLayer::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string RegisterUser(const std::string& username) {
    // Data we are sending to the server.
    chirp::RegisterRequest request;
    request.set_username(username);

    // Container for the data we expect from the server.
    chirp::RegisterReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->Register(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string Chirp(const std::string& username, const std::string& text, const int& parent_id) {
    chirp::ChirpRequest request;
    request.set_username(username);
    request.set_username(text);
    request.set_parent_id(parent_id);

    chirp::ChirpReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->Chirp(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string Follow(const std::string& username, const std::string& to_follow) {
    chirp::FollowRequest request;
    request.set_username(username);
    request.set_to_follow(to_follow);

    chirp::FollowReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->Follow(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string Read(const int& chirp_id) {
    chirp::ReadRequest request;
    request.set_chirp_id(chirp_id);

    chirp::ReadReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->Read(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string Monitor(const std::string& username) {
    chirp::MonitorRequest request;
    request.set_username(username);

    chirp::MonitorReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->Monitor(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  ServiceLayerClient sl_client(grpc::CreateChannel(
      "localhost:50002", grpc::InsecureChannelCredentials()));
  std::string reply = sl_client.Register("test username");
  std::cout << "Received: " << reply << std::endl;

  return 0;
}