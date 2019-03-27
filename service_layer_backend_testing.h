#ifndef CPP_SERVICE_LAYER_BACKEND_TESTING_H_
#define CPP_SERVICE_LAYER_BACKEND_TESTING_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include <unistd.h>
#include "chirp_service_layer.grpc.pb.h"
#include "chirp_service_layer.pb.h"
#include "key_value_backend.h"

// service layer backend class that can strategically call the key value grpc
// client functions like Put, Get, Delete to execute higher-level functions like
// Register, Chirp, Follow, Read, Monitor
class ServiceLayerBackEndTesting {
 public:
  // constructor
  ServiceLayerBackEndTesting();
  // destructor
  ~ServiceLayerBackEndTesting();
  // places/registers a user in the key value storage and returns a string
  // that describes success of the attempt
  std::string RegisterUser(const std::string& username);
  // places a chirp object into key value either on its own or as a reply
  // returns a string describing success of the attempt
  std::string Chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);
  // allows 'username' to follow 'to_follow' by adding 'to_follow' to the set of
  // users 'username' is following. Returns a string describing success of the
  // attempt
  std::string Follow(const std::string& username, const std::string& to_follow);
  // finds the chirp with the given chirp_id in the key value storage
  // and returns a vector of the thread of chirps in reply to that chirp_id
  std::vector<chirp::Chirp> Read(const std::string& chirp_id);
  // allows a constant stream of incoming chirps to be returned to the
  // 'username' who wishes to monitor chirps
  void Monitor(const std::string& username,
               grpc::ServerWriter<chirp::MonitorReply>* stream);

 private:
  // instance of the key value client (grpc) on which to call Put, Get, and
  // Delete
  KeyValueBackEnd key_value_backend_;
};

#endif /*CPP_SERVICE_LAYER_BACKEND_TESTING_H_*/