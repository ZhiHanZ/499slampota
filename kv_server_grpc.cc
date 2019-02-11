#include "kv_server_grpc.h"

grpc::Status KeyValueServer::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
  // TODO: find out how to generate random keys
  std::string key = request->key();
  std::string value = request->value();
  kvbe_.Put(key, value);
  return grpc::Status::OK;
}

grpc::Status KeyValueServer::get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream) {
  chirp::GetRequest request;
  stream->Read(&request);
  const std::vector<std::string>& values = kvbe_.Get(request.key());

  chirp::GetReply reply;
  for(const std::string& val : values) {
    reply.set_value(val);
    const chirp::GetReply& sendingReply = reply;
    stream->Write(sendingReply);
  }
  return grpc::Status::OK;
}

grpc::Status KeyValueServer::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
  kvbe_.DeleteKey(request->key());
  return grpc::Status::OK;
}

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