#include <string>
#include <map>
#include <set>
#include "service_layer_interface.h"
#include "kv_client_grpc.cc"
#include "chirpsl.grpc.pb.h"


// service layer backend class that can strategically call the key value grpc client functions
// like Put, Get, Delete to execute higher-level functions like Register, Chirp, Follow, Read, Monitor
class ServiceLayerBackEnd : public ServiceLayerInterface {
	public:
		// constructor
		ServiceLayerBackEnd();
		// destructor
		~ServiceLayerBackEnd();
		// places/registers a user in the key value storage
		std::string RegisterUser(const std::string& username);
		// places a chirp object into key value storage
		// can be placed in on its own or as a reply
		void Chirp(const std::string& username, const std::string& text, const std::string& parent_id);
		// allows 'username' to follow 'to_follow' by adding 'to_follow' to the set of users that 'username' is following 
		void Follow(const std::string& username, const std::string& to_follow);
		// finds the chirp with the given chirp_id in the key value storage
		std::string Read(const std::string& chirp_id);
		// allows a constant stream of incoming chirps to be returned to the 'username' who wishes to monitor chirps
		std::string Monitor(const std::string& username);
	private:
		// instance of the key value client (grpc) on which to call Put, Get, and Delete
		KeyValueClient kv_client_;
		// a counter that ensures that all of the chirps will have a unique key
		key_counter_;
};

ServiceLayerBackEnd::ServiceLayerBackEnd() {

}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {

}

std::string RegisterUser(const std::string& username) {
	// disallow "newest" as a username, because it is reserved to hold new chirps
	if(username == "newest")
	{
		return "That username is unavailable";
	}
	// check if the username is already taken
	if(kv_client_.Get(username)) {
		return "That username already exists";
	}
	else {
		chirp::User reg_user;
    reg_user.set_username(username);
    std::string serialized_user;
    reg_user.SerializeToString(&serialized_user);
		return kv_client_.Put(username, serialized_user);
	}
}

void Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
	// fill in the chirp object with arguments and store it
  chirp::Chirp ch;
  ch.set_username(username);
  ch.set_text(text);
  std::string chirp_id = "chirp_by " + username + " " + key_counter_;
  ch.set_id(chirp_id);
  ch.set_parent_id(parent_id);
  ch.set_timestamp("");
  // TODO: get the real timestamp
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
	kv_client_.Put(chirp_id, serialized_chirp);
	kv_client_.Put("newest", serialized_chirp);
}

void Follow(const std::string& username, const std::string& to_follow) {
	// grab the current list of followers and add new to_follow to the set
	std::string user_string = kv_client_.Get(username);
	chirp::User usr;
	usr.ParseFromString(&user_string);
	usr.add_following();
	std::string updated_user;
	usr.SerializeToString(&updated_user)
	// update the stored value
	kv_client_.DeleteKey(username);
	kv_client_.Put(username, updated_user);
} 

std::string Read(const int& chirp_id) {
	return kv_client_.Get(chirp_id);
}

// TODO: Figure out how to make monitor work over and over
std::string Monitor(const std::string& username) {
	// look at what in the newest key
	std::string newest_chirp kv_client_.Get("newest");
	chirp::Chirp ch;
	ch.ParseFromString(&newest_chirp);
	//if()
	// TODO: deserialize the chirp that is in newest into 'ch'
	return ch;
}