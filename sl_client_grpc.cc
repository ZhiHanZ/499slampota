#include "sl_client_grpc.h"

// ServiceLayerClient::ServiceLayerClient(std::shared_ptr<grpc::Channel> channel)
//     : stub_(chirp::ServiceLayer::NewStub(channel)) {}

// Assembles the client's payload, sends it and presents the response back
// from the server.
//--->
  std::string ServiceLayerClient::RegisterUser(const std::string& username) {
  // Data we are sending to the server.
  chirp::RegisterRequest request;
  request.set_username(username);

  // Container for the data we expect from the server.
  chirp::RegisterReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  // TODO: fix this
  return "what do i return here since register reply is empty???";
}

void ServiceLayerClient::Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
  chirp::ChirpRequest request;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(parent_id);

  chirp::ChirpReply reply;

  grpc::ClientContext context;

  grpc::Status status = stub_->chirp(&context, request, &reply);
}

void ServiceLayerClient::Follow(const std::string& username, const std::string& to_follow) {
  chirp::FollowRequest request;
  request.set_username(username);
  request.set_to_follow(to_follow);

  chirp::FollowReply reply;

  grpc::ClientContext context;

  grpc::Status status = stub_->follow(&context, request, &reply);
}

std::vector<chirp::Chirp> ServiceLayerClient::Read(const std::string& chirp_id) {
  chirp::ReadRequest request;
  request.set_chirp_id(chirp_id);

  chirp::ReadReply reply;

  grpc::ClientContext context;
  grpc::Status status = stub_->read(&context, request, &reply);
  std::vector<chirp::Chirp> values;
  for(int i = 0; i < reply.chirps_size(); i++)
  {
    values.push_back(reply.chirps(i));
  }
  return values;
}

std::string ServiceLayerClient::Monitor(const std::string& username) {
  chirp::MonitorRequest request;
  request.set_username(username);

  chirp::MonitorReply reply;

  grpc::ClientContext context;

  std::unique_ptr< ::grpc::ClientReader< ::chirp::MonitorReply>> stream_handle = stub_->monitor(&context, request);
}