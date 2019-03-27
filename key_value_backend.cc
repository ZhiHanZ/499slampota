#include "key_value_backend.h"

#include <iostream>

KeyValueBackEnd::KeyValueBackEnd() {
	// initialize the key counter to 0 when the database is created
	std::pair<std::string, std::vector<std::string> > key_counter;
	std::vector<std::string> v;
	v.push_back("0");
	key_counter.first = "keyCounterForDataMap";
	key_counter.second = v;
	data_map_.insert(key_counter);
	// create newest and seen keys
	std::pair<std::string, std::vector<std::string> > newest_pair;
	std::vector<std::string> vector_newest;
	newest_pair.first = "newest";
	newest_pair.second = vector_newest;
	data_map_.insert(newest_pair);
	std::pair<std::string, std::vector<std::string> > seen_pair;
	std::vector<std::string> vector_seen;
	vector_seen.push_back("0");
	seen_pair.first = "size_of_newest";
	seen_pair.second = vector_seen;
	data_map_.insert(seen_pair);
}

KeyValueBackEnd::~KeyValueBackEnd() {}

std::vector<std::string> KeyValueBackEnd::Get(const std::string& key) {
	std::lock_guard<std::mutex> lock(mutex_lock);
	std::map<std::string,std::vector<std::string> >::iterator it;
	it = data_map_.find(key);
	std::vector<std::string> retrieved_vector;
  	if (it != data_map_.end()) {
  		retrieved_vector = data_map_.at(key);
		std::cout << "this vector was retrieved for " << key << ": " << std::endl;
		for (unsigned int i = 0; i <retrieved_vector.size(); i++)
		{
			std::cout << retrieved_vector[0] << std::endl;
		}
  	}
	return retrieved_vector;
}

void KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	std::cout << "put " << key << " and " << value << std::endl;
	std::lock_guard<std::mutex> lock(mutex_lock);
	std::map<std::string,std::vector<std::string> >::iterator it = data_map_.find(key);
	// adding to an existing key key
	if (it != data_map_.end()) {
		std::cout << key << " found in the map already" << std::endl;
   		it->second.push_back(value);
   	// creating a new key
	} else {
		std::pair<std::string, std::vector<std::string> > create_chirp;
		std::vector<std::string> vector_of_data_strings;
		if (value != "") {
			vector_of_data_strings.push_back(value);
		}
		create_chirp.first = key;
		create_chirp.second = vector_of_data_strings;
		data_map_.insert(create_chirp);
	}
}

void KeyValueBackEnd::DeleteKey(const std::string& key) {
	std::cout << "DElete got valled" <<std::endl;
	std::lock_guard<std::mutex> lock(mutex_lock);
	data_map_.erase(key);
}

int KeyValueBackEnd::size() {
	return data_map_.size();
}