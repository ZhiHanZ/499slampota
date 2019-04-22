#ifndef UTILS_STREAM_HELPER_H_
#define UTILS_STREAM_HELPER_H_
#include <google/protobuf/message.h>
#include <grpcpp/grpcpp.h>
#include <chrono>
#include <string>
#include "protos/chirp_service_layer.grpc.pb.h"
#include "protos/chirp_service_layer.pb.h"
#include "regex.h"
using chirp::Chirp;
using chirp::TagChirps;
using chirp::Timestamp;
using std::string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
namespace utils {
// extract all hashtags from one chirp and :q
// Get current micro second
static int64_t GetMicroSec() {
  return duration_cast<microseconds>(system_clock::now().time_since_epoch())
      .count();
}
// Get current second
static int64_t GetSec() {
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return duration_cast<seconds>(now).count();
}
// Get the current time in chirp::Timestamp format
Timestamp GetTime() {
  Timestamp time;
  time.set_seconds(GetSec());
  time.set_useconds(GetMicroSec());
  return time;
}
}  // namespace utils
#endif  // UTILS_STREAM_HELPER_H_
