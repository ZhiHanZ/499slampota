#include "service_layer_server_grpc.h"

grpc::Status ServiceLayerServer::registeruser(
    grpc::ServerContext* context, const chirp::RegisterRequest* request,
    chirp::RegisterReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's
  // data structure
  std::string username = request->username();
  std::string result = service_layer_back_end_.RegisterUser(username);
  if (result == "illegal") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Illegal username.");
  } else if (result == "taken") {
    return grpc::Status(grpc::StatusCode::ALREADY_EXISTS,
                        "User Already Exists.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::chirp(grpc::ServerContext* context,
                                       const chirp::ChirpRequest* request,
                                       chirp::ChirpReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's
  // data structure
  std::string username = request->username();
  std::string text = request->text();
  std::string parent_id = request->parent_id();
  std::string result = service_layer_back_end_.Chirp(username, text, parent_id);
  if (result != "success") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND,
                        "Parent id was not found.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::follow(grpc::ServerContext* context,
                                        const chirp::FollowRequest* request,
                                        chirp::FollowReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's
  // data structure
  std::string username = request->username();
  std::string to_follow = request->to_follow();
  std::string result = service_layer_back_end_.Follow(username, to_follow);
  if (result != "success") {
    return grpc::Status(grpc::StatusCode::NOT_FOUND,
                        "User to follow was not found.");
  }
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServer::read(grpc::ServerContext* context,
                                      const chirp::ReadRequest* request,
                                      chirp::ReadReply* reply) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's
  // data structure
  std::vector<chirp::Chirp> values =
      service_layer_back_end_.Read(request->chirp_id());
  for (chirp::Chirp val : values) {
    chirp::Chirp* created_chirp = reply->add_chirps();
    created_chirp->set_username(val.username());
    created_chirp->set_text(val.text());
    created_chirp->set_id(val.id());
    created_chirp->set_parent_id(val.parent_id());
  }
  if (values.empty()) {
    return grpc::Status(grpc::StatusCode::NOT_FOUND,
                        "That chirp id was not found.");
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServer::monitor(
    grpc::ServerContext* context, const chirp::MonitorRequest* request,
    grpc::ServerWriter<chirp::MonitorReply>* stream) {
  // unwrap the request's fields so that we may pass them to the ServiceLayer's
  // data structure
  std::string username = request->username();
  service_layer_back_end_.Monitor(username, stream);
  return grpc::Status::OK;
}