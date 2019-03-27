#ifndef CPP_KEY_VALUE_CLIENT_GRPC_H_
#define CPP_KEY_VALUE_CLIENT_GRPC_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "chirp_key_value.grpc.pb.h"
#include "key_value_server_grpc.h"

// Key Value Client Class that calls key_value_server_grpc functions
class KeyValueClient {
 public:
  // constructor
  KeyValueClient();
  // destructor
  ~KeyValueClient();
  // function call to the key-value store's server-side grpc version of 'put'
  // to store the arguments 'key' and 'value'
  void Put(const std::string& key, const std::string& value);
  // function call to the key-value store's server-side grpc version of 'get'
  // to retrieve argument 'key' from the backend
  std::vector<std::string> Get(const std::string& key);
  // function call to the key-value store's server-side grpc version of
  // 'deletekey' in order to delete the backend's 'key' and associated value
  void DeleteKey(const std::string& key);

 private:
  // a unique pointer to an instance of the key-value store's server-side class
  // so that we can call it's functions
  std::unique_ptr<chirp::KeyValueStore::Stub> stub_;
};

#endif /*CPP_KEY_VALUE_CLIENT_GRPC_H_*/