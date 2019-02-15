#include "kv_backend.h"

#include <iostream>

// global mutex that keeps the database thread safe
std::mutex mutex_lock;

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
	seen_pair.first = "seen";
	seen_pair.second = vector_seen;
	data_map_.insert(seen_pair);
}

KeyValueBackEnd::~KeyValueBackEnd() {}

std::vector<std::string> KeyValueBackEnd::Get(const std::string& key) {
	std::lock_guard<std::mutex> lock(mutex_lock);
	return data_map_.at(key);
}

void KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	std::lock_guard<std::mutex> lock(mutex_lock);
	std::map<std::string,std::vector<std::string> >::iterator it = data_map_.find(key);
	// check to see if we are updating either of our special key's:
	// "newest" which supports monitor or "keyCounterForDataMap" which ensures unique keys
	if ((key == "keyCounterForDataMap")) {
		// erase the key and rewrite it since only need to store one count at a time
		data_map_.erase(key);
		std::pair<std::string, std::vector<std::string> > create_chirp;
		std::vector<std::string> vector_of_data_strings;
		vector_of_data_strings.push_back(value);
		create_chirp.first = key;
		create_chirp.second = vector_of_data_strings;
		data_map_.insert(create_chirp);
	} else if (it != data_map_.end()) {
   		it->second.push_back(value);
	} else {
		std::pair<std::string, std::vector<std::string> > create_chirp;
		std::vector<std::string> vector_of_data_strings;
		// check if the value is empty (a user is being registered)
		if (value != "") {
			vector_of_data_strings.push_back(value);
		}
		create_chirp.first = key;
		create_chirp.second = vector_of_data_strings;
		data_map_.insert(create_chirp);
	}
}

void KeyValueBackEnd::DeleteKey(const std::string& key) {
	data_map_.erase(key);
}

int KeyValueBackEnd::size() {
	return data_map_.size();
}