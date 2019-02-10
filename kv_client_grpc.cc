#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "key_value_interface.h"
#ifdef BAZEL_BUILD
// TODO: I'm not sure what to put here in place of the proto stuff that hello world uses
#include "protos/chirpkv.grpc.pb.h"
#else
#include "chirpkv.grpc.pb.h"
#endif

// Key Value Client Class that calls kv_server_grpc functions
class KeyValueClient : public KeyValueInterface {
 public:
  KeyValueClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(chirp::KeyValueStore::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  void Put(const std::string& key, const std::string& value) {
    // Data we are sending to the server.
    chirp::PutRequest request;
    request.set_key(key);
    request.set_value(value);

    // Container for the data we expect from the server.
    chirp::PutReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->put(&context, request, &reply);
    
  }

  std::string Get(const std::string& key) {
    chirp::GetRequest request;
    request.set_key(key);

    chirp::GetReply reply;

    grpc::ClientContext context;

    std::unique_ptr<grpc::ClientReaderWriter<chirp::GetRequest, chirp::GetReply> > stream = stub_->get(&context);

  }

  void DeleteKey(const std::string& key) {
    chirp::DeleteRequest request;
    request.set_key(key);

    chirp::DeleteReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->deletekey(&context, request, &reply);
  }

 private:
  std::unique_ptr<chirp::KeyValueStore::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50000). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  KeyValueClient kv_client(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  //std::string reply = 
  kv_client.Put("test key", "test value");
  //std::cout << "Received: " << reply << std::endl;

  return 0;
}
