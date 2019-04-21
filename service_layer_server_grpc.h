#ifndef SERVICE_LAYER_SERVER_GRPC_H_
#define SERVICE_LAYER_SERVER_GRPC_H_

#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include "chirp_service_layer.grpc.pb.h"
#include "chirp_service_layer.pb.h"
#include "service_layer_backend.h"

// Service Layer Server part of grpc that makes calls to the true sl_backend
class ServiceLayerServer final : public chirp::ServiceLayer::Service {
 public:
  // makes a call to the service layer backend's 'RegisterUser' function
  // and returns a grpc Status to denote any errors or success through grpc
  grpc::Status registeruser(grpc::ServerContext* context,
                            const chirp::RegisterRequest* request,
                            chirp::RegisterReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  // and returns a grpc Status to denote any errors or success through grpc
  grpc::Status chirp(grpc::ServerContext* context,
                     const chirp::ChirpRequest* request,
                     chirp::ChirpReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  // and returns a grpc Status to denote any errors or success through grpc
  grpc::Status follow(grpc::ServerContext* context,
                      const chirp::FollowRequest* request,
                      chirp::FollowReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  // and returns a grpc Status to denote any errors or success through grpc
  grpc::Status read(grpc::ServerContext* context,
                    const chirp::ReadRequest* request,
                    chirp::ReadReply* reply) override;
  // makes a call to the service layer backend's 'RegisterUser' function
  // and returns a grpc Status to denote any errors or success through grpc
  grpc::Status monitor(
      grpc::ServerContext* context, const chirp::MonitorRequest* request,
      grpc::ServerWriter<chirp::MonitorReply>* stream) override;
  // makesa a call to backend stream function return streaming data accosiated
  // to given hashtag
  grpc::Status Stream(grpc::ServerContext* context,
                      const chirp::StreamRequest* request,
                      grpc::ServerWriter<chirp::StreamReply>* stream) override;

 private:
  // private instance of service layer's backend so that we can call its
  // functions
  ServiceLayerBackEnd service_layer_back_end_;
};

#endif /*SERVICE_LAYER_SERVER_GRPC_H_*/
