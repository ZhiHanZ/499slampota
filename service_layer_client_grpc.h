#ifndef CPP_SERVICE_LAYER_CLIENT_GRPC_H_
#define CPP_SERVICE_LAYER_CLIENT_GRPC_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "chirp_service_layer.grpc.pb.h"
#include "service_layer_server_grpc.h"

// Service Layer Client Class that calls sl_server_grpc functions
class ServiceLayerClient {
 public:
  // constructor
  ServiceLayerClient();
  // destructor
  ~ServiceLayerClient();
  // call to the server-side grpc's version of 'registeruser'
  // returns a string that describes the success of the attempt to register
  std::string RegisterUser(const std::string& username);
  // call to the server-side grpc's version of 'chirp'
  // returns a string that describes the success of the attempt to chirp
  std::string Chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);
  // call to server-side grpc's version of 'follow' so 'username' follows
  // 'to_follow' returns a string that describes the success of the attempt to
  // follow
  std::string Follow(const std::string& username, const std::string& to_follow);
  // call to the server-side grpc's version of 'read' to read the given
  // 'chirp_id' returns a string that describes the success of the attempt to
  // register
  std::vector<chirp::Chirp> Read(const std::string& chirp_id);
  // call to the server-side grpc's version of 'monitor' to monitor chirps
  // from everyone 'username' is following
  // returns a string that describes the success of the attempt to register
  void Monitor(const std::string& username);

 private:
  // unique pointer to the ServiceLayerServer
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_;
};

#endif /*CPP_SERVICE_LAYER_CLIENT_GRPC_H_*/