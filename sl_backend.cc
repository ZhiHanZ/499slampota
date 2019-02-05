#include <string>
#include <map>
#include "service_layer_interface.h"

// true sl_backend implementation - still a little confused about its role???
class ServiceLayerBackEnd : public ServiceLayerInterface{
	public:
		ServiceLayerBackEnd();
		~ServiceLayerBackEnd();
		void RegisterUser(const std::string& username);
		void Chirp(const std::string& username, const std::string& text, const int& parent_id);
		void Follow(const std::string& username, const std::string& to_follow);
		std::string Read(const int& chirp_id);
		std::string Monitor(const std::string& username);
	private:
		// TODO: i have no idea what the data member here is??? do we even need one?
		//is it a pointer to the kvbe so that we can alter it from here??
};

ServiceLayerBackEnd::ServiceLayerBackEnd() {

}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {

}

void RegisterUser(const std::string& username) {

}

void Chirp(const std::string& username, const std::string& text, const int& parent_id) {

}

void Follow(const std::string& username, const std::string& to_follow) {

} 

std::string Read(const int& chirp_id) {
	return "";
}

std::string Monitor(const std::string& username) {
	return "";
}