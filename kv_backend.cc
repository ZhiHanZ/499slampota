#include <string>
#include <map>
#include "key_value_interface.h"

// true kv_backend implementation that stores all raw data
class KeyValueBackEnd : public KeyValueInterface{
	public:
		KeyValueBackEnd();
		virtual ~KeyValueBackEnd();
		std::string Get(const std::string& key) override;
		void Put(const std::string& key, const std::string& chirp_value) override;
		void DeleteKey(const std::string& key) override;
		int size();
	private:
		// key - string, value - serialized object as a string
		std::map<std::string, std::string> data_map_;	
};

std::string KeyValueBackEnd::Get(const std::string& key) {
	return data_map_.at(key);
}

void KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	std::pair<std::string, std::string> createChirp;
	createChirp.first = key;
	createChirp.second = value;
	data_map_.insert(createChirp);
}

void KeyValueBackEnd::DeleteKey(const std::string& key) {
	data_map_.erase(key);
}

int KeyValueBackEnd::size() {
	return data_map_.size();
}

