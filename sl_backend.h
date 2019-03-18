#ifndef CPP_SL_BACKEND_H_
#define CPP_SL_BACKEND_H_

#include <string>
#include <map>
#include <set>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "kv_client_grpc.h"
#include "chirpsl.grpc.pb.h"

// service layer backend class that can strategically call the key value grpc client functions
// like Put, Get, Delete to execute higher-level functions like Register, Chirp, Follow, Read, Monitor
class ServiceLayerBackEnd {
	public:
		// constructor
		ServiceLayerBackEnd();
		// destructor
		~ServiceLayerBackEnd();
		// places/registers a user in the key value storage
		std::string RegisterUser(const std::string& username);
		// places a chirp object into key value storage
		// can be placed in on its own or as a reply
		std::string Chirp(const std::string& username, const std::string& text, const std::string& parent_id);
		// allows 'username' to follow 'to_follow' by adding 'to_follow' to the set of users that 'username' is following 
		std::string Follow(const std::string& username, const std::string& to_follow);
		// finds the chirp with the given chirp_id in the key value storage
		std::vector<chirp::Chirp> Read(const std::string& chirp_id);
		// allows a constant stream of incoming chirps to be returned to the 'username' who wishes to monitor chirps
		void Monitor(const std::string& username, grpc::ServerWriter<chirp::MonitorReply>* stream);
	private:
		// instance of the key value client (grpc) on which to call Put, Get, and Delete
		KeyValueClient key_value_client_; 
};

#endif /*CPP_SL_BACKEND_H_*/