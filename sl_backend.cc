#include <string>
#include <map>
#include "service_layer_interface.cc"

// true kv_backend implementation that stores all raw data
class ServiceLayerBackEnd : public ServiceLayerInterface{
	public:
		ServiceLayerBackEnd();
		~ServiceLayerBackEnd();
		std::string RegisterUser(const std::string& username);
		std::string Chirp(const std::string& username, const std::string& text, const int& parent_id);
		std::string Follow(const std::string& username, const std::string& to_follow);
		std::string Read(const int& chirp_id);
		std::string Monitor(const std::string& username);
	private:
		// TODO: i have no idea what the data member here is??? do we even need one?
		//is it a pointer to the kvbe so that we can alter it from here??
};

std::string RegisterUser(const std::string& username) {

	return "";
}

std::string Chirp(const std::string& username, const std::string& text, const int& parent_id) {

	return "";
}

std::string Follow(const std::string& username, const std::string& to_follow) {

	return "";
} 

std::string Read(const int& chirp_id) {
	return "";
}

std::string Monitor(const std::string& username) {
	return "";
}