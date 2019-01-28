#include <string>
#include <map>
#include <set>
#include <pair>

class KeyValueDataStore {
	public:
		KeyValueDataStore();
		~KeyValueDataStore();
		int RegisterNewUser(string username);
		int StoreChirp(string username, chirp_text);
		int FollowUser(string username, string to_follow);
	private:
		std::set<string> usernames_;
		// key - username, value - set of usernames they're following
		std::map<int, set<string> > usernames_following_;
		// key - chirpID, value - pair of chirper's username and chirp text
		std::map<int, pair<string, string> > chirps_;
		int number_of_chirps_;
}

KeyValueDataStore::KeyValueDataStore() {
	// TODO: constructor
	number_of_chirps_ = 0;
}

KeyValueDataStore::~KeyValueDataStore() {
	// TODO: destructor??? do i need one
}

int KeyValueDataStore::RegisterNewUser(string username) {
	int error = 1;
	int old_size = usernames_.size();
	usernames_.insert(username);
	if(username_.size == (old_size + 1))
	{
		error = 0;
	}
	return error;
}

int KeyValueDataStore::StoreChirp(string username, chirp_text) {
	// create the inner pair of username and text first
	pair<string, string> user_text_pair;
	user_text_pair.first = username;
	user_text_pair.second = chirp_text;
	// embed it within the pair for the chirps_ map
	pair<int, pair<string, string> > chirp_pair;
	chirp_pair.first = number of chirps+1;
	chirp_pair.second = user_text_pair;
	chirps_.insert(chirp_pair);
	return 0;
}

// returns 0 if all goes well
// returns 1 if they were already following that user
// returns 2 if the string_to_follow doesn't exist

int KeyValueDataStore::FollowUser(string username, string to_follow) {
	//TODO: figure out if i need the return 1 and return 2 cases above???
}