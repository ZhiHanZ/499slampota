#include <string>
#include <map>

// Service Layer Interface class that is the parent class 
// for both the sl_backend and the sl_client_grpc
class ServiceLayerInterface {
	public:
		ServiceLayerInterface();
		virtual ~ServiceLayerInterface();
		virtual std::string RegisterUser(const std::string& username);
		virtual std::string Chirp(const std::string& username, const std::string& text, const int& parent_id);
		virtual std::string Follow(const std::string& username, const std::string& to_follow);
		virtual std::string Read(const int& chirp_id);
		virtual std::string Monitor(const std::string& username);
	private:
		// TODO: still no idea what this is
}; 