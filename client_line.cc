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

DEFINE_string(register, "", "the username you wish to register with");
DEFINE_string(user, "", "the username you wish to login with");
DEFINE_string(chirp, "", "the text you wish to chirp");
DEFINE_string(reply, "", "the id of the chirp you wish to reply to");
DEFINE_string(follow, "", "the username you wish to follow");
DEFINE_string(read, "", "the id of the chirp you wish to read");
DEFINE_bool(monitor, false, "call this to monitor chirps by people you follow");

int main(int argc, char **argv)
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	std::cout << "before the slc!" << std::endl;
	std::cout << FLAGS_register << std::endl;
	std::cout << FLAGS_user << std::endl;
	std::cout << FLAGS_chirp << std::endl;
	std::cout << FLAGS_reply << std::endl;
	std::cout << FLAGS_follow << std::endl;
	std::cout << FLAGS_read << std::endl;
	std::cout << FLAGS_monitor << std::endl;

	ServiceLayerClient slc;
	std::string username = "";
	if(FLAGS_register.size() > 0)
	{
		slc.RegisterUser(FLAGS_register);
	}
	if(FLAGS_user.size() > 0)
	{
		username = FLAGS_user;
	}
	if(FLAGS_chirp.size() > 0) // and the username is not ""
	{
		if(username.size() > 0)
		{
			if(FLAGS_reply.size() > 0)
			{
				slc.Chirp(username, FLAGS_chirp, FLAGS_reply);
			}
			else {
				slc.Chirp(username, FLAGS_chirp, "");
			}
		}
		else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if(FLAGS_follow.size() > 0) // and username is not ""
	{
		if(username.size() > 0)
		{
			slc.Follow(username, FLAGS_follow);
		}
		else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if(FLAGS_read.size() > 0) // and username is not ""
	{
		if(username.size() > 0)
		{
			std::vector<chirp::Chirp> v = slc.Read("chirp_by: stephanie 0");
			for(unsigned int i = 0; i < v.size(); i++)
			{
				std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
			}
		}
		else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if(FLAGS_monitor)  // and username is not ""
	{
		if(username.size() > 0)
		{
			// instantiate a client reader with a stub that connects to sl client which calls monitor on stub
			// Create channel for the stub
			// TODO: Is this the right port to connext to the service layer client grpc?
					// std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
		      // "localhost:50002", grpc::InsecureChannelCredentials());
			// instantiate stub with channel
			// TODO: what do i replace servicelayer with?
      		// std::unique_ptr<chirp::ServiceLayer::Stub> stub_ = chirp::ServiceLayer::NewStub(channel);
      // create context request etc
		    	// chirp::MonitorRequest request;
				  // request.set_username(username);

				  // chirp::MonitorReply reply;

				  // grpc::ClientContext context;
      // TODO: is this the right signature?
					// std::unique_ptr<grpc::ClientReader<chirp::MonitorReply> stream_handle (stub_->monitor(&context, &request));
			slc.Monitor(username);
					// while(stream_handle.Read(&reply))
					// {
					// 	// print it out
					// 	chirp::Chirp ch = reply.chirp();
					// 	std::cout << ch.username() << ": " << ch.text() <<  " with a chirp id of: [" << ch.id() << "]" << std::endl;
					// }
		}
		else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	// =-----------=
	// while(true)
	// {
	// 	std::string s;
	// 	std::cin >> s;
	// 	std::cout << "HEY" << std::endl;
 // 		ServiceLayerClient slc;
	// 	//slc.RegisterUser("stephanie");
	// 	slc.RegisterUser("jill");
	// 	slc.RegisterUser("aliya");
	// 	slc.RegisterUser("krishna");
	// 	slc.Chirp("stephanie", "my first chirp", ""); // chirp_by: stephanie 0
	// 	// slc.Follow("stephanie", "jill");
	// 	// slc.Follow("stephanie", "aliya");
	// 	// slc.Follow("stephanie", "krishna");
	// 	// slc.Monitor("stephanie");
	// 	slc.Chirp("jill", "nice", "chirp_by: stephanie 1"); // chirp_by: jill 2
	// 	slc.Chirp("aliya", "thats how they get you", ""); // chirp_by: aliya 3
	// 	slc.Chirp("aliya", "skeet skeet bang bang", "chirp_by: jill 2"); // chirp_by: aliya 4
	// 	slc.Chirp("krishna", "omy", "chirp_by: jill 2"); // chirp_by: krishna 5
	// 	std::cout << "--------- reading aliya 3 --------" << std::endl;
	// 	std::vector<chirp::Chirp> v = slc.Read("chirp_by: aliya 3");
	// 	for(unsigned int i = 0; i < v.size(); i++)
	// 	{
	// 		std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
	// 	}
	// 	std::cout << "--------- reading jill 2 --------" << std::endl;
	// 	v = slc.Read("chirp_by: jill 2");
	// 	for(unsigned int i = 0; i < v.size(); i++)
	// 	{
	// 		std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
	// 	}
	// 	std::cout << "--------- reading steph 1 --------" << std::endl;
	// 	v = slc.Read("chirp_by: stephanie 1");
	// 	for(unsigned int i = 0; i < v.size(); i++)
	// 	{
	// 		std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
	// 	}
		// test monitor by making the monitor for stephanie
	// }
	return 0;
}

#endif