#include "sl_backend.h"

#include <iostream>


ServiceLayerBackEnd::ServiceLayerBackEnd() {}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {}

std::string ServiceLayerBackEnd::RegisterUser(const std::string& username) {
	// disallow "newest" as a username, because it is reserved to hold new chirps
	if(username == "newest")
	{
		return "That username is unavailable";
	}
	// check if the username is already taken
	if(!((kv_client_.Get(username)).empty())) {
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
  if(parent_id == "")
  {
  	std::cout << "inserting chirp with id: " << chirp_id << " and no parent id" << std::endl;
  }
  else {
  	std::cout << "inserting chirp with id: " << chirp_id << " and parent id: " << parent_id << std::endl;
  }
  std::cout << "line 39" << std::endl;
  ch.set_id(chirp_id);
  std::cout << "line 41" << std::endl;
  ch.set_parent_id(parent_id);
  std::cout << "line 43" << std::endl;
  chirp::Timestamp time;
  //ch.set_timestamp(time);
  // TODO: get the real timestamp
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
	kv_client_.Put(chirp_id, serialized_chirp);
	// kv_client_.Put(parent_id, serialized_chirp);
	// TODO: get the parent id chirp and see if it has a parent id and if so - put the serialized chirp in their parent to - and so on and so on
	std::string pid = ch.parent_id();
	while(pid != "") 
	{
		std::cout << "ths should print once for pid jill and again for pid steph" << std::endl;
		kv_client_.Put(pid, serialized_chirp);
		chirp::Chirp hold;
		hold.ParseFromString(kv_client_.Get(pid)[0]);
		pid = hold.parent_id();
	}
	kv_client_.Put("newest", serialized_chirp);
	key_counter_++;
  std::cout << "line 63" << std::endl;
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
chirp::Chirp ServiceLayerBackEnd::Monitor(const std::string& username) {
	// grab whatever is currently in the newest 
	// so that whenever the chirp id in newest changes we can send it upwards
	std::string current_chirp = kv_client_.Get("newest")[0];
	chirp::Chirp cc;
	cc.ParseFromString(current_chirp);
	std::string old_chirp_id = cc.id();
	std::vector<std::string> following = kv_client_.Get(username);
	std::string newest_chirp;
	chirp::Chirp ch;
	while(true)
	{
		newest_chirp = kv_client_.Get("newest")[0];
		ch.ParseFromString(newest_chirp);
		// see if it's a new chirp
		if(ch.id() != old_chirp_id)
		{
			// mark a new chirp as now old
			old_chirp_id = ch.id();
			// check if the chirp is by someone the user is following
			bool relevant = false;
			for(int i = 0; i < following.size(); i++)
			{
				std::cout << following[i];
				if(following[i] == ch.username())
				{
					relevant = true;
				}
			}
			// if the chirp was both new and relevant - send it!
			if(relevant)
			{
				return ch;
			}
		}

	}
}