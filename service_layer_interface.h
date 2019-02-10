#include <string>
#include <map>
#include "kv_client_grpc.cc"

// Service Layer Interface class that is the parent class 
// for both the sl_backend and the sl_client_grpc
class ServiceLayerInterface {
	public:
		// constructor
		ServiceLayerInterface();
		// destructor
		virtual ~ServiceLayerInterface();
		// places/registers a user in the key value storage
		virtual void RegisterUser(const std::string& username) = 0;
		// places a chirp object into key value storage
		// can be placed in on its own or as a reply
		virtual void Chirp(const std::string& username, const std::string& text, const std::string& parent_id) = 0;
		// allows 'username' to follow 'to_follow' by adding 'to_follow' to the set of users that 'username' is following
		virtual void Follow(const std::string& username, const std::string& to_follow) = 0;
		// finds the chirp with the given chirp_id in the key value storage
		virtual std::string Read(const std::string& chirp_id) = 0;
		// allows a constant stream of incoming chirps to be returned to the 'username' who wishes to monitor chirps
		virtual std::string Monitor(const std::string& username) = 0;
	private:
		// instance of the key value client (grpc) on which to call Put, Get, and Delete
		KeyValueClient kv_client_;
		// a counter that ensures that all of the chirps will have a unique key
		int key_counter_;
}; 

ServiceLayerInterface::ServiceLayerInterface() {
	key_counter_ = 0;
}

ServiceLayerInterface::~ServiceLayerInterface() {
	
}