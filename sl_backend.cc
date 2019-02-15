#include "sl_backend.h"

#include <iostream>


ServiceLayerBackEnd::ServiceLayerBackEnd() {}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {}

std::string ServiceLayerBackEnd::RegisterUser(const std::string& username) {
	// disallow "newest" as a username, because it is reserved to hold new chirps
	if((username == "newest") || (username == "keyCounterForDataMap"))
	{
		return "That username is unavailable";
	}
	// check if the username is already taken
	if(!((key_value_client_.Get(username)).empty())) {
		return "That username already exists";
	}
	else {
		key_value_client_.Put(username, "");
		return "success";
	}
}

void ServiceLayerBackEnd::Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
	// fill in a chirp object with given arguments
  chirp::Chirp ch;
  ch.set_username(username);
  ch.set_text(text);
  // retrieve the key_counter from the KeyValueStore to create a unique chirp id
  std::string key_counter = key_value_client_.Get("keyCounterForDataMap")[0];
  std::string chirp_id = "chirp_by: " + username + " " + key_counter;
  ch.set_id(chirp_id);
  ch.set_parent_id(parent_id);
  // increment the key_counter and restore it
  int int_key_counter = std::stoi(key_counter);
  int_key_counter++;
  std::string updated_key_counter = std::to_string(int_key_counter);
  key_value_client_.Put("keyCounterForDataMap", updated_key_counter);
  // serialize the chirp before sending it to the KeyValueDataStore
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
	key_value_client_.Put(chirp_id, serialized_chirp);
	// link this tweet to any tweet it was replying to/in a thread with
	std::string pid = ch.parent_id();
	while(pid != "") 
	{
		key_value_client_.Put(pid, serialized_chirp);
		chirp::Chirp hold;
		hold.ParseFromString(key_value_client_.Get(pid)[0]);
		pid = hold.parent_id();
	}
	// place the chirp in the "newest" key so that monitoring users may access it
	key_value_client_.Put("newest", serialized_chirp);
}

void ServiceLayerBackEnd::Follow(const std::string& username, const std::string& to_follow) {
	key_value_client_.Put(username, to_follow);
} 

std::vector<chirp::Chirp> ServiceLayerBackEnd::Read(const std::string& chirp_id) {
	// retrieve the vector of strings from the KeyValueStore 
	std::vector<chirp::Chirp> values;
	for(const std::string& val : key_value_client_.Get(chirp_id)) {
		chirp::Chirp ch;
		// and parse the strings into Chirps to send back into the ServiceLayer
		ch.ParseFromString(val);
		values.push_back(ch);
	}
	return values;
}


void ServiceLayerBackEnd::Monitor(const std::string& username, grpc::ServerWriter<chirp::MonitorReply>* stream) {
	// keep track of whatever is currently in the "newest" key
	// so that whenever the chirp id in "newest" changes we send it to the monitoring user
	std::string current_chirp_string = key_value_client_.Get("newest")[0];
	chirp::Chirp current_chirp;
	current_chirp.ParseFromString(current_chirp_string);
	std::string old_chirp_id = current_chirp.id();
	std::vector<std::string> following = key_value_client_.Get(username);
	std::string newest_chirp;
	chirp::Chirp candidate_chirp;
	while(true)
	{
		newest_chirp = key_value_client_.Get("newest")[0];
		// check if the chirp is new
		candidate_chirp.ParseFromString(newest_chirp);		
		if(candidate_chirp.id() != old_chirp_id)
		{
			// mark a new chirp as now old
			old_chirp_id = candidate_chirp.id();
			// check if the chirp is by someone the user is following
			bool relevant = false;
			for(unsigned int i = 0; i < following.size(); i++)
			{
				std::cout << following[i];
				if(following[i] == candidate_chirp.username())
				{
					relevant = true;
				}
			}
			// if the chirp was both new and relevant - send it
			if(relevant)
			{
			  chirp::MonitorReply reply;
			  reply.set_allocated_chirp(&candidate_chirp);
			  const chirp::MonitorReply& kSendingReply = reply;
			  stream->Write(kSendingReply);
			}
		}
	}
}