#include "service_layer_backend_testing.h"
#include <iostream>
#include <optional>
#include "utils/regex.h"
#include "utils/stream_helper.h"
using utils::GetTime;
using utils::HashParser;

ServiceLayerBackEndTesting::ServiceLayerBackEndTesting() {}

ServiceLayerBackEndTesting::~ServiceLayerBackEndTesting() {}

std::string ServiceLayerBackEndTesting::RegisterUser(
    const std::string& username) {
  // disallow "newest" as a username, because it is reserved to hold new chirps
  if ((username == "newest") || (username == "keyCounterForDataMap") ||
      username == "registered user") {
    return "illegal";
  }
  // check if the username is already taken
  std::vector<std::string> v = key_value_backend_.Get(username);
  if (!(v.empty())) {
    return "taken";
  } else {
    key_value_backend_.Put(username, "registered user");
    return "success";
  }
}

std::string ServiceLayerBackEndTesting::Chirp(const std::string& username,
                                              const std::string& text,
                                              const std::string& parent_id) {
  // fill in a chirp object with given arguments
  chirp::Chirp ch;
  ch.set_username(username);
  ch.set_text(text);
  // retrieve the key_counter from the KeyValueStore to create a unique chirp id
  std::string key_counter = key_value_backend_.Get("keyCounterForDataMap")[0];
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
      key_value_backend_.Get(parent_chirp_id);
  if ((parent_chirp_id != "") && (parent_validity.size() == 0)) {
    return "parent id does not exist";
  }
  key_value_backend_.DeleteKey("keyCounterForDataMap");
  key_value_backend_.Put("keyCounterForDataMap", updated_key_counter);
  key_value_backend_.Put(chirp_id, serialized_chirp);
  while (parent_chirp_id != "") {
    key_value_backend_.Put(parent_chirp_id, serialized_chirp);
    chirp::Chirp hold;
    hold.ParseFromString(key_value_backend_.Get(parent_chirp_id)[0]);
    parent_chirp_id = hold.parent_id();
  }
  // place the chirp in the "newest" key so that monitoring users may access it
  key_value_backend_.DeleteKey("newest");
  key_value_backend_.Put("newest", serialized_chirp);
  // Parse the hashtag info from chirp ans send them to database
  ChirpStream(ch);
  return "success";
}

std::string ServiceLayerBackEndTesting::Follow(const std::string& username,
                                               const std::string& to_follow) {
  std::vector<std::string> does_to_follow_exist =
      key_value_backend_.Get(to_follow);
  if (does_to_follow_exist.empty()) {
    return "user to follow does not exist";
  }
  key_value_backend_.Put(username, to_follow);
  return "success";
}

std::vector<chirp::Chirp> ServiceLayerBackEndTesting::Read(
    const std::string& chirp_id) {
  // retrieve the vector of strings from the KeyValueStore
  std::vector<chirp::Chirp> requested_chirps;
  std::vector<std::string> list_of_chirp_strings =
      key_value_backend_.Get(chirp_id);
  for (const std::string& c : list_of_chirp_strings) {
    chirp::Chirp requested_chirp;
    // and parse the strings into Chirps to send back into the ServiceLayer
    requested_chirp.ParseFromString(c);
    requested_chirps.push_back(requested_chirp);
  }
  return requested_chirps;
}

void ServiceLayerBackEndTesting::Monitor(
    const std::string& username,
    grpc::ServerWriter<chirp::MonitorReply>* stream) {
  // keep track of whatever is currently in the "newest" key
  // so that whenever the chirp id in "newest" changes we send it to the
  // monitoring user
  std::vector<std::string> recent_chirps = key_value_backend_.Get("newest");
  std::string old_chirp_id;
  if (recent_chirps.size() != 0) {
    std::string current_chirp_string = recent_chirps[recent_chirps.size() - 1];
    chirp::Chirp current_chirp;
    current_chirp.ParseFromString(current_chirp_string);
    old_chirp_id = current_chirp.id();
  } else {
    old_chirp_id = "everything is new";
  }

  std::vector<std::string> following = key_value_backend_.Get(username);
  std::string newest_chirp;
  chirp::Chirp candidate_chirp;
  while (true) {
    recent_chirps = key_value_backend_.Get("newest");
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
// append current time to chirp and put chirps into their tag categories
void ServiceLayerBackEndTesting::ChirpStream(chirp::Chirp& chirp) {
  // Add timestamp to current chirp
  auto time = GetTime();
  chirp.mutable_timestamp()->set_seconds(time.seconds());
  chirp.mutable_timestamp()->set_useconds(time.useconds());
  auto text = chirp.text();
  auto all_tags = HashParser(text);
  std::string serial_chirp;
  chirp.SerializeToString(&serial_chirp);
  for (const auto& tag : all_tags) {
    auto vec = key_value_backend_.Get(tag);
    // ensure that tag always map to the most updated chirp
    if (vec.size() == 0) {
      key_value_backend_.Put(tag, serial_chirp);
    } else {
      key_value_backend_.DeleteKey(tag);
      key_value_backend_.Put(tag, serial_chirp);
    }
  }
}

// Get hashtag's chirp through tag and return std::nullopt if
// such tag have no chirp
std::optional<chirp::Chirp> ServiceLayerBackEndTesting::GetTagInfo(
    const std::string& tag) {
  auto vec = key_value_backend_.Get(tag);
  if (!vec.empty()) {
    chirp::Chirp chirp;
    chirp.ParseFromString(vec[0]);
    return chirp;
  }
  return std::nullopt;
}

// Stream service, keep streaming request hash tags
// and send results to reply
// Can synchronize with StreamBuffer function to buffer
// reply information to a vector
// NOTE: You should not open buffer mode if you do not
// need to buffer chirp information unless stream will be blocked
// example usage:
// ServiceLayerBackEndTesting service;
// std::vector<chirp::Chirp> buffer;// (created buffer)
// chirp::StreamRequest stream_request;
// stream_request.set_hashtag("#1");
// chirp::StreamReply stream_reply;
// auto th2 = service.StreamBuffer(&stream_reply, buffer);
// service.OpenBuffer();
// service.SetNumStreamLoop(100);
// service.Stream(&stream_request, &stream_reply);
// th2.join();
void ServiceLayerBackEndTesting::Stream(const chirp::StreamRequest* request,
                                        chirp::StreamReply* reply) {
  auto tag = request->hashtag();
  auto time_interval = stream_refresh_timeval_;
  int64_t curr_loop = 0;
  auto time = GetTime();
  // stream_refresh_times by default is -1 which means a infinite loop
  while (curr_loop != stream_refresh_times_) {
    std::this_thread::sleep_for(time_interval);
    auto chirp = GetTagInfo(tag);
    // once time updated put updated chirp into reply
    // if buffer mode is open, send this message to buffer and wait
    // for the finish of buffer process
    if (chirp && (*chirp).timestamp().useconds() > time.useconds()) {
      std::unique_lock<std::mutex> monitor_lk(stream_mutex_);
      if (stream_buff_mode_) {
        stream_buf_signal_.wait(monitor_lk, [& stream_flag = stream_flag_] {
          return !stream_flag;
        });
      }
      StreamSet(reply, *chirp);
      stream_flag_ = true;
      monitor_lk.unlock();
      stream_buf_signal_.notify_one();
      time.set_useconds((*chirp).timestamp().useconds());
    }
    if (stream_refresh_times_ != -1) {
      curr_loop++;
    }
  }
  std::lock_guard<std::mutex> lk(stream_mutex_);
  stream_exit_flag_ = true;
  // once we finish refresh time go back to default value
  stream_refresh_times_ = -1;
  stream_buf_signal_.notify_one();
  return;
}
//  Stream Buffer Service puts reply chirp into buffer
std::thread ServiceLayerBackEndTesting::StreamBuffer(
    const chirp::StreamReply* reply, vector<chirp::Chirp>& buffer) {
  std::thread thr(
      [this, reply, &buffer]() { StreamBufferHelper(reply, buffer); });
  return thr;
}
//  Stream Buffer Service puts reply chirp into buffer
void ServiceLayerBackEndTesting::StreamBufferHelper(
    const chirp::StreamReply* reply, vector<chirp::Chirp>& buffer) {
  //  lock condition: either get a chirp or we finished streaming
  auto lock_cond = [& exit_flag = stream_exit_flag_,
                    &stream_flag = stream_flag_] {
    return exit_flag || stream_flag;
  };
  while (true) {
    std::unique_lock<std::mutex> monitor_lk(stream_mutex_);
    stream_buf_signal_.wait(monitor_lk, lock_cond);
    if (stream_exit_flag_) return;
    chirp::Chirp curr = reply->chirp();
    buffer.push_back(curr);
    stream_flag_ = false;
    monitor_lk.unlock();
    stream_buf_signal_.notify_all();
  }
}
// set reply through reply_chrp (no need for memory management
void ServiceLayerBackEndTesting::StreamSet(chirp::StreamReply* reply,
                                           const chirp::Chirp& reply_chirp) {
  reply->mutable_chirp()->set_id(reply_chirp.id());
  reply->mutable_chirp()->set_username(reply_chirp.username());
  reply->mutable_chirp()->set_text(reply_chirp.text());
  reply->mutable_chirp()->set_parent_id(reply_chirp.parent_id());
  reply->mutable_chirp()->mutable_timestamp()->set_seconds(
      reply_chirp.timestamp().seconds());
  reply->mutable_chirp()->mutable_timestamp()->set_useconds(
      reply_chirp.timestamp().useconds());
}
