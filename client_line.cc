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
	ServiceLayerClient service_layer_client;
	std::string username = "";
	if (FLAGS_register.size() > 0) {
		service_layer_client.RegisterUser(FLAGS_register);
	}
	if (FLAGS_user.size() > 0) {
		username = FLAGS_user;
	}
	if (FLAGS_chirp.size() > 0) {
		if (username.size() > 0) {
			if (FLAGS_reply.size() > 0) {
				service_layer_client.Chirp(username, FLAGS_chirp, FLAGS_reply);
			} else {
				service_layer_client.Chirp(username, FLAGS_chirp, "");
			}
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if (FLAGS_follow.size() > 0) {
		if (username.size() > 0) {
			service_layer_client.Follow(username, FLAGS_follow);
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if (FLAGS_read.size() > 0) {
		if (username.size() > 0) {
			std::vector<chirp::Chirp> vector_to_read = service_layer_client.Read(FLAGS_read);
			for (unsigned int i = 0; i < vector_to_read.size(); i++) {
				std::cout << vector_to_read[i].username() << ": " << std::endl;
		    std::cout << "\"" << vector_to_read[i].text() << "\"" << std::endl;
		    std::cout << "with a chirp id of: [" << vector_to_read[i].id() << "]" << std::endl;
		    std::cout << std::endl;
			}
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	if (FLAGS_monitor) {
		if (username.size() > 0) {
			service_layer_client.Monitor(username);
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	return 0;
}

#endif