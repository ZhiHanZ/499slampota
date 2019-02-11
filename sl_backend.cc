#include "sl_backend.h"

ServiceLayerBackEnd::ServiceLayerBackEnd() {}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {}

std::string ServiceLayerBackEnd::RegisterUser(const std::string& username) {
	// disallow "newest" as a username, because it is reserved to hold new chirps
	if(username == "newest")
	{
		return "That username is unavailable";
	}
	// check if the username is already taken
	if(kv_client_.Get(username).size() != 0) {
		return "That username already exists";
	}
	else {
		kv_client_.Put(username, "");
		return "success";
	}
}

void ServiceLayerBackEnd::Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
	// fill in the chirp object with arguments and store it
  chirp::Chirp ch;
  ch.set_username(username);
  ch.set_text(text);
  std::string chirp_id = "chirp_by: " + username + " " + std::to_string(key_counter_);
  ch.set_id(chirp_id);
  ch.set_parent_id(parent_id);
  chirp::Timestamp time;
  //ch.set_timestamp(time);
  // TODO: get the real timestamp
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
	kv_client_.Put(chirp_id, serialized_chirp);
	kv_client_.Put(parent_id, serialized_chirp);
	kv_client_.Put("newest", serialized_chirp);
}

void ServiceLayerBackEnd::Follow(const std::string& username, const std::string& to_follow) {
	kv_client_.Put(username, to_follow);
} 

std::vector<chirp::Chirp> ServiceLayerBackEnd::Read(const std::string& chirp_id) {
	std::vector<chirp::Chirp> values;
	for(const std::string& val : kv_client_.Get(chirp_id)) {
		chirp::Chirp ch;
		ch.ParseFromString(val);
		values.push_back(ch);
	}
	return values;
}

// TODO: Figure out how to make monitor work over and over
std::string ServiceLayerBackEnd::Monitor(const std::string& username) {
	// look at what in the newest key
	std::string newest_chirp = kv_client_.Get("newest")[0];
	chirp::Chirp ch;
	ch.ParseFromString(newest_chirp);
	// TODO: deserialize the chirp that is in newest into 'ch'
	return ch.text();
}