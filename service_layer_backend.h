#ifndef CPP_SERVICE_LAYER_BACKEND_H_
#define CPP_SERVICE_LAYER_BACKEND_H_

#include <grpcpp/grpcpp.h>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include "chirp_service_layer.grpc.pb.h"
#include "chirp_service_layer.pb.h"
#include "key_value_client_grpc.h"
using std::condition_variable;
using std::vector;
using std::chrono::milliseconds;
using std::literals::chrono_literals::operator""ms;

// service layer backend class that can strategically call the key value grpc
// client functions like Put, Get, Delete to execute higher-level functions like
// Register, Chirp, Follow, Read, Monitor
class ServiceLayerBackEnd {
 public:
  // constructor
  ServiceLayerBackEnd();
  // destructor
  ~ServiceLayerBackEnd();
  // places/registers a user in the key value storage
  std::string RegisterUser(const std::string& username);
  // places a chirp object into key value storage
  // can be placed in on its own or as a reply
  std::string Chirp(const std::string& username, const std::string& text,
                    const std::string& parent_id);
  // allows 'username' to follow 'to_follow' by adding 'to_follow' to the set of
  // users that 'username' is following
  std::string Follow(const std::string& username, const std::string& to_follow);
  // finds the chirp with the given chirp_id in the key value storage
  std::vector<chirp::Chirp> Read(const std::string& chirp_id);
  // allows a constant stream of incoming chirps to be returned to the
  // 'username' who wishes to monitor chirps
  void Monitor(const std::string& username,
               grpc::ServerWriter<chirp::MonitorReply>* stream);
  // Get the current chirp in tag
  // if there do not have a tag return a empty string
  std::pair<chirp::Chirp, bool> GetTagInfo(const std::string& tag);
  // Keep track of current stream infomation given hashTag
  void Stream(const chirp::StreamRequest* request,
              grpc::ServerWriter<chirp::StreamReply>* reply);
  // buffer messages in chirps through another thread
  std::thread StreamBuffer(const chirp::StreamReply* reply,
                           vector<chirp::Chirp>& buffer);
  // buffer messages in chirps
  void StreamBufferHelper(const chirp::StreamReply* reply,
                          vector<chirp::Chirp>& buffer);
  // allow StreamBuffer service to buffer received Stream data
  void OpenBuffer() { stream_buff_mode_ = true; }
  // close Buffer service for stream
  void CloseBuffer() { stream_buff_mode_ = false; }
  //  allow You to Get current stream loop times, -1 represents that
  //  It loop forever
  auto GetNumStreamLoop() { return stream_refresh_times_; }
  //  Set stream loop times
  auto SetNumStreamLoop(int64_t loop_num) { stream_refresh_times_ = loop_num; }
  // Get stream polling time interval default is 5ms
  auto GetStreamTimeVal() { return stream_refresh_timeval_; }
  // Change Stream refresh time interval
  auto SetRefreshTimeVal(int wait_time) { stream_refresh_timeval_ = 5ms; }

 private:
  // allocates chirps into several different tags
  void ChirpStream(chirp::Chirp& chirp);
  // allocate the value of StreamReply in stack
  void StreamSet(chirp::StreamReply* reply, const chirp::Chirp& reply_chirp);
  // instance of the key value client (grpc) on which to call Put, Get, and
  // Delete
  KeyValueClient key_value_client_;
  // the number of refresh times during Streaming service
  // default value -1 represents a infinite loop
  // Except we press ctrl + C, it won't stop
  int64_t stream_refresh_times_ = -1;
  // Stream service polling time : default value is 5ms
  milliseconds stream_refresh_timeval_ = 5ms;
  //  synchronize between Stream() service and StreamBuffer() service
  //  when we need to buffer messages received from Stream()
  mutable std::mutex stream_mutex_;
  //  synchronize between Stream() service and StreamBuffer() service
  //  when we need to buffer messages received from Stream()
  std::condition_variable stream_buf_signal_;
  // If we want to buffer stream data to a vector, we should set it to be true
  // else it will be false
  bool stream_buff_mode_ = false;
  // It will be true if stream service received an updated message
  bool stream_flag_ = false;
  // It will be true if stream service exits
  bool stream_exit_flag_ = false;
};

#endif /*CPP_SERVICE_LAYER_BACKEND_H_*/
