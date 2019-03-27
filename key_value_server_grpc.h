#ifndef CPP_KEY_VALUE_SERVER_GRPC_H_
#define CPP_KEY_VALUE_SERVER_GRPC_H_

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chirp_key_value.grpc.pb.h"
#include "key_value_backend.h"

// Key Value Server part of grpc that makes calls to the key_value_backend
class KeyValueServer final : public chirp::KeyValueStore::Service {
 public:
  // function that takes in a request containing the key and value to insert
  // via a call to the backend's Put(), returns a grpc Status
  grpc::Status put(grpc::ServerContext* context,
                   const chirp::PutRequest* request,
                   chirp::PutReply* reply) override;
  // function that takes in a request containing the key and value to insert
  // via a call to the backend's Get(), stores the retrieved value in the reply
  // argument and returns a grpc Status
  grpc::Status get(grpc::ServerContext* context,
                   grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>*
                       stream) override;
  // function that takes in a request containing the key to delete from backend
  // via a call to the backend's DeleteKey(), returns a grpc Status
  grpc::Status deletekey(grpc::ServerContext* context,
                         const chirp::DeleteRequest* request,
                         chirp::DeleteReply* reply) override;

 private:
  // instance of the key-value backend class so that we can call its functions
  KeyValueBackEnd key_value_back_end_;
};

#endif /*CPP_KEY_VALUE_SERVER_GRPC_H_*/