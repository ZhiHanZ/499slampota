#include "sl_client_grpc.h"

ServiceLayerClient::ServiceLayerClient() {
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
      "localhost:50002", grpc::InsecureChannelCredentials());
  stub_ = chirp::ServiceLayer::NewStub(channel);
}

ServiceLayerClient::~ServiceLayerClient() {}

  std::string ServiceLayerClient::RegisterUser(const std::string& username) {
  // set the fields of the request to pass to ServiceLayerServer
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
  grpc::ClientContext context;
  // call ServiceLayerServer's registeruser function
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  // TODO: fix this
  return "what do i return here since register reply is empty???";
}

void ServiceLayerClient::Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
  // set the fields of the request to pass to ServiceLayerServer
  chirp::ChirpRequest request;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(parent_id);
  chirp::ChirpReply reply;
  grpc::ClientContext context;
  // call ServiceLayerServer's chirp function
  grpc::Status status = stub_->chirp(&context, request, &reply);
  std::cout << "got back here" << std::endl;
}

void ServiceLayerClient::Follow(const std::string& username, const std::string& to_follow) {
  // set the fields of the request to pass to ServiceLayerServer
  chirp::FollowRequest request;
  request.set_username(username);
  request.set_to_follow(to_follow);

  chirp::FollowReply reply;

  grpc::ClientContext context;
  // call ServiceLayerServer's follow function
  grpc::Status status = stub_->follow(&context, request, &reply);
}

std::vector<chirp::Chirp> ServiceLayerClient::Read(const std::string& chirp_id) {
  // set the fields of the request to pass to ServiceLayerServer
  chirp::ReadRequest request;
  request.set_chirp_id(chirp_id);
  chirp::ReadReply reply;
  grpc::ClientContext context;
  // call ServiceLayerServer's read function
  grpc::Status status = stub_->read(&context, request, &reply);
  // create a vector to hold the possible thread of chirps and their replies
  std::vector<chirp::Chirp> values;
  for(int i = 0; i < reply.chirps_size(); i++)
  {
    values.push_back(reply.chirps(i));
  }
  return values;
}

void ServiceLayerClient::Monitor(const std::string& username) {
  // set the fields of the request to pass to ServiceLayerServer
  chirp::MonitorRequest request;
  request.set_username(username);
  chirp::MonitorReply reply;
  grpc::ClientContext context;
  // call ServiceLayerServer's monitor function
  std::unique_ptr<grpc::ClientReader<chirp::MonitorReply> > stream_handle (stub_->monitor(&context, request));
  //wait on incoming messages from the ServiceLayerServer
  while(stream_handle->Read(&reply))
  {
    // print the incoming messages out as formatted chirps
    chirp::Chirp ch = reply.chirp();
    std::cout << ch.username() << ": " << std::endl;
    std::cout << "\"" << ch.text() << "\"" << std::endl;
    std::cout << "with a chirp id of: [" << ch.id() << "]" << std::endl;
    std::cout << std::endl;
  }
}