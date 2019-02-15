#include "kv_server_grpc.h"

#include <iostream>

grpc::Status KeyValueServer::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
  //unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  std::string key = request->key();
  std::string value = request->value();
  // call KeyValueBackend's Put function
  key_value_back_end_.Put(key, value);
  return grpc::Status::OK;
}

grpc::Status KeyValueServer::get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream) {
  //unwrap the request's fields so that we may pass them to the ServiceLayer's data structure
  chirp::GetRequest request;
  stream->Read(&request);
  // call KeyValueBackend's Get function
  const std::vector<std::string>& kValues = key_value_back_end_.Get(request.key());

  chirp::GetReply reply;
  for (const std::string& kVal : kValues) {
    reply.set_value(kVal);
    const chirp::GetReply& kSendingReply = reply;
    stream->Write(kSendingReply);
  }
  return grpc::Status::OK;
}

grpc::Status KeyValueServer::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
  // call KeyValueBackend's DeleteKey function
  key_value_back_end_.DeleteKey(request->key());
  return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("localhost:50000");
  KeyValueServer key_value_server;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&key_value_server);
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