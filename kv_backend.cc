#include <string>
#include <map>

// KeyValueBackEnd class to store all serialized data as bytes
class KeyValueBackEnd {
	public:
		KeyValueBackEnd();
		~KeyValueBackEnd();
		std::string Get(const std::string& key) const;
		int Put(const std::string& key, const std::string& chirp_value);
		int DeleteKey(const std::string& key);
		int size() const;
	private:
		// map to store data of key - std::string, value - serialized chirp object
		std::map<std::string, std::string> chirp_map_;
};

KeyValueBackEnd::KeyValueBackEnd() {

}

KeyValueBackEnd::~KeyValueBackEnd() {
	// TODO: destructor??? do i need one
	
}

// retrieve values with a string key
std::string KeyValueBackEnd::Get(const std::string& key) const {
	return chirp_map_.at(key);
}

// insert a key and value into the chirp_map_
int KeyValueBackEnd::Put(const std::string& key, const std::string& chirp_value) {
	std::pair<std::string, std::string> createPair;
	createPair.first = key;
	createPair.second = chirp_value;
	chirp_map_.insert(createPair);
	return 0;
}

// delete key from the chirp_map_
int KeyValueBackEnd::DeleteKey(const std::string& key) {
	chirp_map_.erase(key);
	return 0;
}

// function to keep track of the size of the backend
int KeyValueBackEnd::size() const {
	return chirp_map_.size();
}
