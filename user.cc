#include <string>

class User {
	public:
		User();
		~User();
		string username();
	private:
		std::string username;
};