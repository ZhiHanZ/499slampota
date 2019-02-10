#include <iostream>
#include <memory>
#include <string>
#include <deque>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
// #include "kv_backend.cc"
// #include "kv_backend.cc"
#include "protos/chirpkv.grpc.pb.h"
#else
#include "chirpkv.grpc.pb.h"
#include "kv_backend.cc"
#endif

// Key Value Server part of grpc that makes calls to the true kv_backend
class KeyValueServer final : public chirp::KeyValueStore::Service {
  public:
    grpc::Status put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
      // TODO: find out how to generate random keys
      std::string key = request->key();
      std::string value = request->value();
      kvbe_.Put(key, value);
      return grpc::Status::OK;
    }

    grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* get(grpc::ServerContext* context) {
    //grpc::Status get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream) {
      //grpc::Status get(ServerContext* context) override {
      grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream;
      chirp::GetRequest request;
      stream->Read(&request);
      //std::deque<std::string>& values;
      std::string s = kvbe_.Get(request.key());
      //values.push_back(s);

      chirp::GetReply reply;
      // for(const std::string& val : values) {
      //   reply.set_value(val);
      //   const chirp::GetReply& sendingReply = reply;
      //   stream->Write(sendingReply);
      // }
      reply.set_value(kvbe_.Get(request.key()));
      return stream;
    }

    grpc::Status deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
      kvbe_.DeleteKey(request->key());
      return grpc::Status::OK;
    }
  private:
    KeyValueBackEnd kvbe_;
};

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  KeyValueServer kvs;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&kvs);
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