#include "kv_client_grpc.h"

KeyValueClient::KeyValueClient() {}
KeyValueClient::~KeyValueClient() {}

// Assembles the client's payload, sends it and presents the response back
// from the server.
void KeyValueClient::Put(const std::string& key, const std::string& value) {
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

std::vector<std::string> KeyValueClient::Get(const std::string& key) {
  chirp::GetRequest request;
  request.set_key(key);

  chirp::GetReply reply;

  grpc::ClientContext context;
  grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream;

  std::unique_ptr<grpc::ClientReaderWriter<chirp::GetRequest, chirp::GetReply> > stream_handle (stub_->get(&context));
  stream_handle->Write(request);
  std::vector<std::string> values;
  while(stream_handle->Read(&reply))
  {
    values.push_back(reply.value());
  }
  return values;
}

void KeyValueClient::DeleteKey(const std::string& key) {
  chirp::DeleteRequest request;
  request.set_key(key);

  chirp::DeleteReply reply;

  grpc::ClientContext context;

  grpc::Status status = stub_->deletekey(&context, request, &reply);
}