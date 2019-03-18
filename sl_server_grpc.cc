#include "sl_server_grpc.h"

#include <iostream>

grpc::Status ServiceLayerServer::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::string username = request->username();
  std::string result = service_layer_back_end_.RegisterUser(username);
  if (result == "illegal") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Illegal username.");
  } else if (result == "taken") {
    return grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "User Already Exists.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::string username = request->username();
  std::string text = request->text();
  std::string parent_id = request->parent_id();
  std::string result = service_layer_back_end_.Chirp(username, text, parent_id);
  if (result != "success") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Parent id was not found.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::string username = request->username();
  std::string to_follow = request->to_follow();
  std::string result = service_layer_back_end_.Follow(username, to_follow);
  if (result != "success") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "User to follow was not found.");
  }
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServer::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::vector<chirp::Chirp> values = service_layer_back_end_.Read(request->chirp_id());
  for (chirp::Chirp val : values) {
    chirp::Chirp* created_chirp = reply->add_chirps();
    created_chirp->set_username(val.username());
    created_chirp->set_text(val.text());
    created_chirp->set_id(val.id());
    created_chirp->set_parent_id(val.parent_id());
  }
  if (values.empty()) {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "That chirp id was not found.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, grpc::ServerWriter<chirp::MonitorReply>* stream) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::string username = request->username();
  service_layer_back_end_.Monitor(username, stream);
  return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("localhost:50002");
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