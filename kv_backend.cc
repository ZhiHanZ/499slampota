#include "kv_backend.h"

KeyValueBackEnd::KeyValueBackEnd() {}

KeyValueBackEnd::~KeyValueBackEnd() {}

std::vector<std::string> KeyValueBackEnd::Get(const std::string& key) {
	return data_map_.at(key);
}

void KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	// see if key is already in the map
	std::map<std::string,std::vector<std::string> >::iterator it = data_map_.find(key);
	if(it != data_map_.end())
	{
   		//element found;
   		it->second.push_back(value);
	}
	else{
		std::pair<std::string, std::vector<std::string> > createChirp;
		std::vector<std::string> v;
		if(value != "") // value is empty because we are just registering the user
		{
			v.push_back(value);
		}
		createChirp.first = key;
		createChirp.second = v;
		data_map_.insert(createChirp);
	}
}

void KeyValueBackEnd::DeleteKey(const std::string& key) {
	data_map_.erase(key);
}

int KeyValueBackEnd::size() {
	return data_map_.size();
}