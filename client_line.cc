#ifndef CPP_CLIENT_LINE_H_
#define CPP_CLIENT_LINE_H_
#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <gflags/gflags.h>
#include "chirp_service_layer.grpc.pb.h"
#include "chirp_service_layer.pb.h"
#include "service_layer_client_grpc.h"

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
	// register
	if (FLAGS_register.size() > 0) {
		std::string result = service_layer_client.RegisterUser(FLAGS_register);
		if (result == "taken" || result == "illegal") {
			std::cout << "That username is unavailable." << std::endl;
		} else {
			std::cout << "Success registering that user." << std::endl;
		}
	}
	// username declaration
	bool logged_in = false;
	if (FLAGS_user.size() > 0) {
		username = FLAGS_user;
		std::string result = service_layer_client.RegisterUser(username);
		if (result == "illegal") {
			std::cout << "You are not a registered user, and that username is unavailable." << std::endl;
		}
		if (result == "success") {
			std::cout << username << " was not registered, but has now been registered." << std::endl;
			logged_in = true;
		}
		else { // username was already registered
			logged_in = true;
		}
	}
	// chirp
	if (FLAGS_chirp.size() > 0) {
		if (logged_in) {
			std::string valid_parent_id;
			if (FLAGS_reply.size() > 0) {
				valid_parent_id = service_layer_client.Chirp(username, FLAGS_chirp, FLAGS_reply);
			} else {
				valid_parent_id = service_layer_client.Chirp(username, FLAGS_chirp, "");
			}
			if (valid_parent_id == "success") {
				std::cout << "Success." << std::endl;
			} else {
				std::cout << "That parent id is nonexistant." << std::endl;
			}
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	// follow
	if (FLAGS_follow.size() > 0) {
		if (logged_in) {
			std::string result = service_layer_client.Follow(username, FLAGS_follow);
			if (result == "success") {
				std::cout << username << " is now following " << FLAGS_follow << "." << std::endl;
			} else {
				std::cout << "The user you are trying to follow does not exist." << std::endl;
			}
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	// read
	if (FLAGS_read.size() > 0) {
		if (logged_in) {
			std::vector<chirp::Chirp> vector_to_read = service_layer_client.Read(FLAGS_read);
			if (vector_to_read.empty()) {
				std::cout << "That chirp id does not exist." << std::endl;
			} else {
				for (unsigned int i = 0; i < vector_to_read.size(); i++) {
					std::cout << vector_to_read[i].username() << ": " << std::endl;
			    std::cout << "\"" << vector_to_read[i].text() << "\"" << std::endl;
			    std::cout << "with a chirp id of: [" << vector_to_read[i].id() << "]" << std::endl;
			    std::cout << std::endl;
				}
			}
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	// monitor
	if (FLAGS_monitor) {
		if (logged_in) {
			service_layer_client.Monitor(username);
		} else {
			std::cout << "Please login with a username." << std::endl;
		}
	}
	return 0;
}

#endif