#ifndef CPP_KV_BACKEND_H_
#define CPP_KV_BACKEND_H_

#include <string>
#include <map>
#include <vector>
#include <mutex>

// true kv_backend implementation that stores all raw data
class KeyValueBackEnd {
	public:
		// constructor
		KeyValueBackEnd();
		// destructor
		virtual ~KeyValueBackEnd();
		// get function that is used to retrieve values from the data_map_
		std::vector<std::string> Get(const std::string& key);
		// get function that is used to insert values from the data_map_
		void Put(const std::string& key, const std::string& chirp_value);
		// delete function that is used to remove values from the data_map_
		void DeleteKey(const std::string& key);
		// size function for testing purposes
		int size();
	private:
		// key - string, value - vector of strings
		std::map<std::string, std::vector<std::string> > data_map_;	
};

#endif /*CPP_KV_BACKEND_H_*/