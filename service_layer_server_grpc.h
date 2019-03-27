#ifndef CPP_SERVICE_LAYER_SERVER_GRPC_H_
#define CPP_SERVICE_LAYER_SERVER_GRPC_H_

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chirp_service_layer.grpc.pb.h"
#include "service_layer_backend.h"

// Service Layer Server part of grpc that makes calls to the true sl_backend
class ServiceLayerServer final : public chirp::ServiceLayer::Service {
 public:
  // makes a call to the service layer backend's 'RegisterUser' function
  grpc::Status registeruser(grpc::ServerContext* context,
                            const chirp::RegisterRequest* request,
                            chirp::RegisterReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  grpc::Status chirp(grpc::ServerContext* context,
                     const chirp::ChirpRequest* request,
                     chirp::ChirpReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  grpc::Status follow(grpc::ServerContext* context,
                      const chirp::FollowRequest* request,
                      chirp::FollowReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  grpc::Status read(grpc::ServerContext* context,
                    const chirp::ReadRequest* request,
                    chirp::ReadReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  grpc::Status monitor(
      grpc::ServerContext* context, const chirp::MonitorRequest* request,
      grpc::ServerWriter<chirp::MonitorReply>* stream) override;

 private:
  // private instance of service layer's backend so that we can call it's
  // functions
  ServiceLayerBackEnd service_layer_back_end_;
};

#endif /*CPP_SERVICE_LAYER_SERVER_GRPC_H_*/