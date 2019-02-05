#include <cstddef>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "gtest/gtest.h"
#include "kv_client_grpc.cc"


TEST(KV_client_grpc, PutGetDeleteGRPC) {
	KeyValueClient kv_client(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));
  std::string reply = kv_client.Put("test key", "test value");
  std::cout << "Received: " << reply << std::endl;
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}