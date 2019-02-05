#include <string>
#include <map>

// Service Layer Interface class that is the parent class 
// for both the sl_backend and the sl_client_grpc
class ServiceLayerInterface {
	public:
		ServiceLayerInterface();
		virtual ~ServiceLayerInterface();
		virtual void RegisterUser(const std::string& username) = 0;
		virtual void Chirp(const std::string& username, const std::string& text, const int& parent_id) = 0;
		virtual void Follow(const std::string& username, const std::string& to_follow) = 0;
		virtual std::string Read(const int& chirp_id) = 0;
		virtual std::string Monitor(const std::string& username) = 0;
	private:
		// TODO: still no idea what this is
}; 

ServiceLayerInterface::ServiceLayerInterface() {

}

ServiceLayerInterface::~ServiceLayerInterface() {
	
}