#include <string>
#include <map>

// Key Value Interface class that is the parent class 
// for both the kv_backend and the kv_client_grpc
class KeyValueInterface {
	public:
		KeyValueInterface();
		virtual ~KeyValueInterface();
		virtual std::string Get(const std::string& key);
		virtual std::string Put(const std::string& key, const std::string& value);
		virtual std::string DeleteKey(const std::string& key);
	private:
		std::map<std::string, std::string> chirp_map_;
};