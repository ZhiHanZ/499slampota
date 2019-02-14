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
	// gflags::ParseCommandLineFlags(&argc, &argv, true);
	// std::cout << "before the slc!" << std::endl;
	// std::cout << FLAGS_register << std::endl;
	// std::cout << FLAGS_user << std::endl;
	// std::cout << FLAGS_chirp << std::endl;
	// std::cout << FLAGS_reply << std::endl;
	// std::cout << FLAGS_follow << std::endl;
	// std::cout << FLAGS_read << std::endl;
	// std::cout << FLAGS_monitor << std::endl;
	
	// ServiceLayerClient slc;
	// std::string username = "";
	// if(FLAGS_register.size() > 0)
	// {
	// 	slc.RegisterUser(FLAGS_register);
	// }
	// if(FLAGS_user.size() > 0)
	// {
	// 	username = FLAGS_user;
	// }
	// if(FLAGS_chirp.size() > 0) // and the username is not ""
	// {
	// 	if(username.size() > 0)
	// 	{
	// 		if(FLAGS_reply.size() > 0)
	// 		{
	// 			slc.Chirp(username, FLAGS_chirp, FLAGS_reply);
	// 		}
	// 		else {
	// 			slc.Chirp(username, FLAGS_chirp, "");
	// 		}
	// 	}
	// 	else {
	// 		std::cout << "Please login with a username." << std::endl;
	// 	}
	// }
	// if(FLAGS_follow.size() > 0) // and username is not ""
	// {
	// 	if(username.size() > 0)
	// 	{
	// 		slc.Follow(username, FLAGS_follow);
	// 	}
	// 	else {
	// 		std::cout << "Please login with a username." << std::endl;
	// 	}
	// }
	// if(FLAGS_read.size() > 0) // and username is not ""
	// {
	// 	if(username.size() > 0)
	// 	{
	// 		std::vector<chirp::Chirp> v = slc.Read("chirp_by: stephanie 0");
	// 		for(unsigned int i = 0; i < v.size(); i++)
	// 		{
	// 			std::cout << v[i].username() <<  " : " << v[i].text() << std::endl;
	// 		}
	// 	}
	// 	else {
	// 		std::cout << "Please login with a username." << std::endl;
	// 	}
	// }
	// if(FLAGS_monitor)  // and username is not ""
	// {
	// 	if(username.size() > 0)
	// 	{
	// 		// instantiate a client reader with a stub that connects to sl client which calls monitor on stub
	// 		// Create channel for the stub
	// 		// TODO: Is this the right port to connext to the service layer client grpc?
	// 				// std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
	// 	      // "localhost:50002", grpc::InsecureChannelCredentials());
	// 		// instantiate stub with channel
	// 		// TODO: what do i replace servicelayer with?
 //      		// std::unique_ptr<chirp::ServiceLayer::Stub> stub_ = chirp::ServiceLayer::NewStub(channel);
 //      // create context request etc
	// 	    	// chirp::MonitorRequest request;
	// 			  // request.set_username(username);

	// 			  // chirp::MonitorReply reply;

	// 			  // grpc::ClientContext context;
 //      // TODO: is this the right signature?
	// 				// std::unique_ptr<grpc::ClientReader<chirp::MonitorReply> stream_handle (stub_->monitor(&context, &request));
	// 		slc.Monitor(username);
	// 				// while(stream_handle.Read(&reply))
	// 				// {
	// 				// 	// print it out
	// 				// 	chirp::Chirp ch = reply.chirp();
	// 				// 	std::cout << ch.username() << ": " << ch.text() <<  " with a chirp id of: [" << ch.id() << "]" << std::endl;
	// 				// }
	// 	}
	// 	else {
	// 		std::cout << "Please login with a username." << std::endl;
	// 	}
	// }
	// =-----------=
	ServiceLayerClient slc;
	slc.RegisterUser("stephanie");
	slc.RegisterUser("tester");
	slc.Follow("tester", "jill");
	slc.Follow("tester", "aliya");
	slc.Follow("tester", "krishna");
	slc.Follow("tester", "stephanie");
	slc.Chirp("stephanie", "first tweet", "");
	slc.Monitor("tester");
	std::cout << "finished!" << std::endl;
	
	return 0;
}

#endif