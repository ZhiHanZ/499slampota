#include "service_layer_backend.h"

#include <iostream>

ServiceLayerBackEnd::ServiceLayerBackEnd() {}

ServiceLayerBackEnd::~ServiceLayerBackEnd() {}

std::string ServiceLayerBackEnd::RegisterUser(const std::string& username) {
  // disallow "newest" as a username, because it is reserved to hold new chirps
  if ((username == "newest") || (username == "keyCounterForDataMap") ||
      username == "registered user") {
    return "illegal";
  }
  // check if the username is already taken
  std::vector<std::string> v = key_value_client_.Get(username);
  if (!(v.empty())) {
    return "taken";
  } else {
    key_value_client_.Put(username, "registered user");
    return "success";
  }
}

std::string ServiceLayerBackEnd::Chirp(const std::string& username,
                                       const std::string& text,
                                       const std::string& parent_id) {
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
  // serialize the chirp before sending it to the KeyValueDataStore
  std::string serialized_chirp;
  ch.SerializeToString(&serialized_chirp);
  // link this tweet to any tweet it was replying to/in a thread with
  std::string parent_chirp_id = ch.parent_id();
  std::vector<std::string> parent_validity =
      key_value_client_.Get(parent_chirp_id);
  if ((parent_chirp_id != "") && (parent_validity.size() == 0)) {
    return "parent id does not exist";
  }
  key_value_client_.DeleteKey("keyCounterForDataMap");
  key_value_client_.Put("keyCounterForDataMap", updated_key_counter);
  key_value_client_.Put(chirp_id, serialized_chirp);
  while (parent_chirp_id != "") {
    key_value_client_.Put(parent_chirp_id, serialized_chirp);
    chirp::Chirp hold;
    hold.ParseFromString(key_value_client_.Get(parent_chirp_id)[0]);
    parent_chirp_id = hold.parent_id();
  }
  // place the chirp in the "newest" key so that monitoring users may access it
  key_value_client_.DeleteKey("newest");
  key_value_client_.Put("newest", serialized_chirp);
  return "success";
}

std::string ServiceLayerBackEnd::Follow(const std::string& username,
                                        const std::string& to_follow) {
  std::vector<std::string> does_to_follow_exist =
      key_value_client_.Get(to_follow);
  if (does_to_follow_exist.empty()) {
    return "user to follow does not exist";
  }
  key_value_client_.Put(username, to_follow);
  return "success";
}

std::vector<chirp::Chirp> ServiceLayerBackEnd::Read(
    const std::string& chirp_id) {
  // retrieve the vector of strings from the KeyValueStore
  std::vector<chirp::Chirp> values;
  for (const std::string& val : key_value_client_.Get(chirp_id)) {
    chirp::Chirp ch;
    // and parse the strings into Chirps to send back into the ServiceLayer
    ch.ParseFromString(val);
    values.push_back(ch);
  }
  return values;
}

void ServiceLayerBackEnd::Monitor(
    const std::string& username,
    grpc::ServerWriter<chirp::MonitorReply>* stream) {
  // keep track of whatever is currently in the "newest" key
  // so that whenever the chirp id in "newest" changes we send it to the
  // monitoring user
  std::vector<std::string> recent_chirps = key_value_client_.Get("newest");
  std::string old_chirp_id;
  if (recent_chirps.size() != 0) {
    std::string current_chirp_string = recent_chirps[recent_chirps.size() - 1];
    chirp::Chirp current_chirp;
    current_chirp.ParseFromString(current_chirp_string);
    old_chirp_id = current_chirp.id();
  } else {
    old_chirp_id = "everything is new";
  }
  std::vector<std::string> following = key_value_client_.Get(username);
  std::string newest_chirp;
  chirp::Chirp candidate_chirp;
  while (true) {
    recent_chirps = key_value_client_.Get("newest");
    if (recent_chirps.size() == 0) {
      continue;
    }
    newest_chirp = recent_chirps[recent_chirps.size() - 1];
    candidate_chirp.ParseFromString(newest_chirp);
    if (candidate_chirp.id() != old_chirp_id) {
      // mark a new chirp as now old
      old_chirp_id = candidate_chirp.id();
      // check if the chirp is by someone the user is following
      bool relevant = false;
      for (unsigned int i = 0; i < following.size(); i++) {
        if (following[i] == candidate_chirp.username()) {
          relevant = true;
        }
      }
      // if the chirp was both new and relevant - send it
      if (relevant) {
        chirp::Chirp* this_chirp = new chirp::Chirp();
        this_chirp->CopyFrom(candidate_chirp);
        chirp::MonitorReply reply;
        reply.set_allocated_chirp(this_chirp);
        chirp::MonitorReply sendingReply = reply;
        stream->Write(sendingReply);
      }
    }
  }
}