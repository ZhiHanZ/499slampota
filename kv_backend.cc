#include <string>
#include <map>

class KeyValueBackEnd {
	public:
		KeyValueBackEnd();
		~KeyValueBackEnd();
		std::string Get(std::string key);
		int Put(std::string key, std::string chirp_value);
		int DeleteKey(std::string key);
		int size();
	private:
		// key - std::string, value - serialized chirp object
		std::map<std::string, std::string> chirp_map_;
		
};

KeyValueBackEnd::KeyValueBackEnd() {

}

KeyValueBackEnd::~KeyValueBackEnd() {
	// TODO: destructor??? do i need one
	
}

std::string KeyValueBackEnd::Get(std::string key) {
	return chirp_map_.at(key);
}

int KeyValueBackEnd::Put(std::string key, std::string chirp_value) {
	std::pair<std::string, std::string> createChirp;
	createChirp.first = key;
	createChirp.second = chirp_value;
	chirp_map_.insert(createChirp);
	return 0;
}

int KeyValueBackEnd::DeleteKey(std::string key) {
	chirp_map_.erase(key);
	return 0;
}

int KeyValueBackEnd::size() {
	return chirp_map_.size();
}
