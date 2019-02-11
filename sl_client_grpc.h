#ifndef CPP_SL_CLIENT_GRPC_H_
#define CPP_SL_CLIENT_GRPC_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "chirpsl.grpc.pb.h"
#include "service_layer_interface.h"

// Service Layer Client Class that calls sl_server_grpc functions
class ServiceLayerClient : public ServiceLayerInterface {
	 public:
		  // constructor that takes in a channel
		  ServiceLayerClient(std::shared_ptr<grpc::Channel> channel)
		      : stub_(chirp::ServiceLayer::NewStub(channel)) {};
		  // function call to the server-side grpc's version of 'registeruser'
		  std::string RegisterUser(const std::string& username);
		  // function call to the server-side grpc's version of 'chirp'
		  void Chirp(const std::string& username, const std::string& text, const std::string& parent_id);
		  // function call to the server-side grpc's version of 'follow'
		  void Follow(const std::string& username, const std::string& to_follow);
		  // function call to the server-side grpc's version of 'read'
		  std::vector<chirp::Chirp> Read(const std::string& chirp_id);
		  // function call to the server-side grpc's version of 'monitor'
		  std::string Monitor(const std::string& username);
		 private:
		  // unique pointer to the ServiceLayerServer
		  std::unique_ptr<chirp::ServiceLayer::Stub> stub_;
};

#endif /*CPP_SL_CLIENT_GRPC_H_*/