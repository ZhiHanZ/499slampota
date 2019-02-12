#ifndef CPP_CLIENT_LINE_H_
#define CPP_CLIENT_LINE_H_
#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include <gflags/gflags.h>

#include "chirpsl.grpc.pb.h"
#include "chirpsl.pb.h"
#include "sl_client_grpc.h"

DEFINE_string(options, "register,user,chirp,reply,follow,read,monitor",
                 "comma-separated list of options a user may execute in 'chirp' menu");

int main(int argc, char **argv)
{
	while(true)
	{
		std::string s;
		std::cin >> s;
		std::cout << "HEY" << std::endl;
 		ServiceLayerClient slc;
		slc.RegisterUser("stephanie");
		slc.RegisterUser("jill");
		slc.RegisterUser("aliya");
		slc.RegisterUser("krishna");
		slc.Chirp("stephanie", "my first chirp", ""); // chirp_by: stephanie 0
		slc.Follow("stephanie", "jill");
		slc.Follow("stephanie", "aliya");
		slc.Follow("stephanie", "krishna");
		slc.Monitor("stephanie");
		slc.Chirp("jill", "nice", "chirp_by: stephanie 0"); // chirp_by: jill 1
		slc.Chirp("aliya", "thats how they get you", ""); // chirp_by: aliya 2
		slc.Chirp("aliya", "skeet skeet bang bang", "chirp_by: jill 1"); // chirp_by: aliya 3
		slc.Chirp("krishna", "omy", "chirp_by: jill 1"); // chirp_by: krishna 4
		std::cout << "--------- reading aliya 2 --------" << std::endl;
		std::vector<chirp::Chirp> v = slc.Read("chirp_by: aliya 2");
		for(unsigned int i = 0; i < v.size(); i++)
		{
			std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
		}
		std::cout << "--------- reading jill 1 --------" << std::endl;
		std::vector<chirp::Chirp> v = slc.Read("chirp_by: jill 1");
		for(unsigned int i = 0; i < v.size(); i++)
		{
			std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
		}
		std::cout << "--------- reading steph 0 --------" << std::endl;
		v = slc.Read("chirp_by: stephanie 0");
		for(unsigned int i = 0; i < v.size(); i++)
		{
			std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
		}
		// test monitor by making the monitor for stephanie
	}
	return 0;
}

#endif