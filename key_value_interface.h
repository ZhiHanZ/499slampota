#include <string>
#include <map>

// Key Value Interface class that is the parent class 
// for both the kv_backend and the kv_client_grpc
class KeyValueInterface {
	public:
		// constructor
		KeyValueInterface();
		// destructor
		virtual ~KeyValueInterface() = 0;
		// returns string value stored with 'key' in map
		virtual std::string Get(const std::string& key) = 0;
		// places a key value pair in the map
		virtual void Put(const std::string& key, const std::string& value) = 0;
		// deletes a key and therefore value from the map
		virtual void DeleteKey(const std::string& key) = 0;
	private:
		// data map that holds all raw string data
		std::map<std::string, std::string> data_map_;
};