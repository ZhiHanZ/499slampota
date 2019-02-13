#include "kv_backend.h"
#include <iostream>

std::mutex mtx;

KeyValueBackEnd::KeyValueBackEnd() {
	// initialize the key counter to 0 when the database is created
	std::pair<std::string, std::vector<std::string> > keyCounter;
	std::vector<std::string> v;
	v.push_back("0");
	keyCounter.first = "keyCounterForDataMap";
	keyCounter.second = v;
	data_map_.insert(keyCounter);
}

KeyValueBackEnd::~KeyValueBackEnd() {}

std::vector<std::string> KeyValueBackEnd::Get(const std::string& key) {
	std::lock_guard<std::mutex> lock(mtx);
	return data_map_.at(key);
}

void KeyValueBackEnd::Put(const std::string& key, const std::string& value) {
	std::lock_guard<std::mutex> lock(mtx);
	std::map<std::string,std::vector<std::string> >::iterator it = data_map_.find(key);
	// check to see if we are updating either of our special key's:
	// "newest" which supports monitor or "keyCounterForDataMap" which ensures unique keys
	if((key == "newest") || (key == "keyCounterForDataMap"))
	{
		std::cout << "updating newest or key count!! w " << value << std::endl;
		// erase the key and rewrite it 
		// since both will only need to store one object at a time
		data_map_.erase(key);
		std::pair<std::string, std::vector<std::string> > createChirp;
		std::vector<std::string> v;
		v.push_back(value);
		createChirp.first = key;
		createChirp.second = v;
		data_map_.insert(createChirp);
	}
	// otherwise - check to see if the key already exists and we should append
	else if(it != data_map_.end())
	{
   		it->second.push_back(value);
	}
	// otherwise - we need to create a new pair to insert into the data_map_
	else{
		std::pair<std::string, std::vector<std::string> > createChirp;
		std::vector<std::string> v;
		// check if the calue is empty (a user is being registered)
		if(value != "")
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