#ifndef CPP_SL_SERVER_GRPC_H_
#define CPP_SL_SERVER_GRPC_H_

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "chirpsl.grpc.pb.h"
#include "sl_backend.cc"

// Service Layer Server part of grpc that makes calls to the true sl_backend
class ServiceLayerServer final : public chirp::ServiceLayer::Service {
  public:
  	// makes a call to the service layer backend's 'RegisterUser' function 
    grpc::Status registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply);
    // makes a call to the service layer backend's 'RegisterUser' function 
    grpc::Status chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply);
    // makes a call to the service layer backend's 'RegisterUser' function 
    grpc::Status follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply);
    // makes a call to the service layer backend's 'RegisterUser' function 
    grpc::Status read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply);
    // makes a call to the service layer backend's 'RegisterUser' function 
    grpc::Status monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply);
  private:
  	// private instance of service layer's backend so that we can call it's functions
    ServiceLayerBackEnd slbe_;
    // a counter to tack onto the end of our data_map_ keys to ensure they are unique
    int key_counter_ = 0;
};

#endif /*CPP_SL_SERVER_GRPC_H_*/