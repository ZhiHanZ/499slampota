#include <string>
#include <map>
#include "key_value_interface.cc"

// true kv_backend implementation that stores all raw data
class KeyValueBackEnd : public KeyValueInterface{
	public:
		KeyValueBackEnd();
		~KeyValueBackEnd();
		std::string Get(const std::string& key);
		std::string Put(const std::string& key, const std::string& chirp_value);
		std::string DeleteKey(const std::string& key);
	private:
		// key - std::string, value - serialized chirp or user object
		std::map<std::string, std::string> chirp_map_;	
};

std::string KeyValueBackEnd::Get(const std::string& key) {
	return chirp_map_.at(key);
}

std::string KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	std::pair<std::string, std::string> createChirp;
	createChirp.first = key;
	createChirp.second = value;
	chirp_map_.insert(createChirp);
	return "";
}

std::string KeyValueBackEnd::DeleteKey(const std::string& key) {
	chirp_map_.erase(key);
	return "";
}