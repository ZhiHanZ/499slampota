#ifndef CPP_CLIENT_LINE_H_
#define CPP_CLIENT_LINE_H_
#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>

#include "chirpsl.grpc.pb.h"
#include "chirpsl.pb.h"
#include "sl_client_grpc.h"

int main(int argc, char **argv)
{
	while(true)
	{ 
		std::string s;
		std::cin >> s;
		std::cout << "HEY" << std::endl;
 		ServiceLayerClient slc(grpc::CreateChannel(
      "localhost:50002", grpc::InsecureChannelCredentials()));
		slc.RegisterUser("stephanie");
		slc.RegisterUser("jill");
		slc.Chirp("stephanie", "my first chirp", "");
		slc.Follow("stephanie", "jill");
		slc.Chirp("jill", "nice", "chirp_by: stephanie 0");
		std::vector<chirp::Chirp> v = slc.Read("chirp_by: jill 1");
		for(unsigned int i = 0; i < v.size(); i++)
		{
			std::cout << v[i].username() <<  " : " << v[i].text();
		}
	}
	return 0;
}

#endif