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
	if(!((kv_client_.Get(username)).empty())) {
		return "That username already exists";
	}
	else {
		kv_client_.Put(username, "");
		return "success";
	}
}

void ServiceLayerBackEnd::Chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
	// fill in a chirp object with given arguments
  chirp::Chirp ch;
  ch.set_username(username);
  ch.set_text(text);
  // retrieve the keyCounter from the KeyValueStore to create a unique chirp id
  std::string keyCounter = kv_client_.Get("keyCounterForDataMap")[0];
  std::string chirp_id = "chirp_by: " + username + " " + keyCounter;
  ch.set_id(chirp_id);
  ch.set_parent_id(parent_id);
  // increment the keyCounter and restore it
  int iKeyCounter = std::stoi(keyCounter);
  iKeyCounter++;
  std::string updatedKeyCounter = std::to_string(iKeyCounter);
  kv_client_.Put("keyCounterForDataMap", updatedKeyCounter);
  if(parent_id == "")
  {
  	std::cout << "sl_backend chirp: " << chirp_id << " w no parent id" << std::endl;
  }
  else {
  	std::cout << "sl_backend chirp: " << chirp_id << " w parent id: " << parent_id << std::endl;
  }
  std::cout << "line 39" << std::endl;
  // serialize the chirp before sending it to the KeyValueDataStore
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
	kv_client_.Put(chirp_id, serialized_chirp);
	// link this tweet to any tweet it was replying to/in a thread with
	std::string pid = ch.parent_id();
	while(pid != "") 
	{
		std::cout << "THIS SHOULD BE PRINTING!!!" << std::endl;
		kv_client_.Put(pid, serialized_chirp);
		chirp::Chirp hold;
		hold.ParseFromString(kv_client_.Get(pid)[0]);
		pid = hold.parent_id();
	}
	// place the chirp in the "newest" key so that monitoring users may access it
	kv_client_.Put("newest", serialized_chirp);
  std::cout << "line 63" << std::endl;
}

void ServiceLayerBackEnd::Follow(const std::string& username, const std::string& to_follow) {
	kv_client_.Put(username, to_follow);
} 

std::vector<chirp::Chirp> ServiceLayerBackEnd::Read(const std::string& chirp_id) {
	// retrieve the vector of strings from the KeyValueStore 
	std::vector<chirp::Chirp> values;
	for(const std::string& val : kv_client_.Get(chirp_id)) {
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
	if(kv_client_.Get("newest").empty())
		{
			std::cout <<"Og NO EMPTY" << std::endl;
		}
	std::string current_chirp = kv_client_.Get("newest")[0];
	chirp::Chirp cc;
	cc.ParseFromString(current_chirp);
	std::string old_chirp_id = cc.id();
	std::vector<std::string> following = kv_client_.Get(username);
	std::string newest_chirp;
	chirp::Chirp ch;
	while(true)
	{
		if(kv_client_.Get("newest").empty())
		{
			std::cout <<"OH NO EMPTY" << std::endl;
		}
		newest_chirp = kv_client_.Get("newest")[0];
		// check if the chirp is new
		ch.ParseFromString(newest_chirp);		
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
				chirp::MonitorReply reply;
			  reply.set_allocated_chirp(&ch);
			  const chirp::MonitorReply& sendingReply = reply;
			  stream->Write(sendingReply);
			}
		}
	}
}