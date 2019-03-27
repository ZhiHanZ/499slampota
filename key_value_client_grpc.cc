#include "key_value_client_grpc.h"

KeyValueClient::KeyValueClient() {
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials());
  stub_ = chirp::KeyValueStore::NewStub(channel);
}

KeyValueClient::~KeyValueClient() {}

void KeyValueClient::Put(const std::string& key, const std::string& value) {
  // set the fields of the request to pass to KeyValueServer
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  grpc::ClientContext context;
  // call KeyValueServer's put function
  grpc::Status status = stub_->put(&context, request, &reply);
}

std::vector<std::string> KeyValueClient::Get(const std::string& key) {
  // set the fields of the request to pass to KeyValueServer
  chirp::GetRequest request;
  request.set_key(key);
  chirp::GetReply reply;
  grpc::ClientContext context;
  // call KeyValyeServer's get function
  std::unique_ptr<grpc::ClientReaderWriter<chirp::GetRequest, chirp::GetReply> >
      stream_handle(stub_->get(&context));
  stream_handle->Write(request);
  std::vector<std::string> values;
  while (stream_handle->Read(&reply)) {
    values.push_back(reply.value());
  }
  return values;
}

void KeyValueClient::DeleteKey(const std::string& key) {
  // set the fields of the request to pass to KeyValueServer
  chirp::DeleteRequest request;
  request.set_key(key);
  chirp::DeleteReply reply;
  grpc::ClientContext context;
  // call KeyValyeServer's deletekey function
  grpc::Status status = stub_->deletekey(&context, request, &reply);
}