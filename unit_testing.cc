
#include <gtest/gtest.h>
#include <cstddef>
#include <string>
#include <vector>
#include "chirp_service_layer.grpc.pb.h"
#include "key_value_backend.h"
#include "service_layer_backend_testing.h"

using std::string;
namespace unittest {
// Can send a chirp automaticly given name time in ms and the number of chirps
void AutoChirp(ServiceLayerBackEndTesting& service, const std::string& name,
               int micro, int chirp_num) {
  int i = 0;
  while (i != chirp_num) {
    string text{"Hello World + #1 " + std::to_string(i)};
    std::this_thread::sleep_for(milliseconds(micro));
    auto str = service.Chirp(name, text, "");
    i++;
  }
}
}  // namespace unittest

// tests for the Key Value Backend's Put, Get, and DeleteKey functions
TEST(KV_backend, PutGetDelete) {
  // set-up kvbe with 'keyCounter', 'newest', 'newest_count' keys
  KeyValueBackEnd kvbe;
  std::vector<std::string> a;
  a.push_back("a");
  std::vector<std::string> b;
  b.push_back("b");
  std::vector<std::string> c;
  c.push_back("c");
  // test the Put() functions when the key does not yet exist
  kvbe.Put("a", "a");
  EXPECT_EQ(4, kvbe.size());
  kvbe.Put("b", "b");
  EXPECT_EQ(5, kvbe.size());
  kvbe.Put("c", "c");
  EXPECT_EQ(6, kvbe.size());
  // test the Get() functions
  EXPECT_EQ(a, kvbe.Get("a"));
  EXPECT_EQ(b, kvbe.Get("b"));
  EXPECT_EQ(c, kvbe.Get("c"));
  // test the Put() functions when the key does not yet exist
  kvbe.Put("a", "b");
  a.push_back("b");
  EXPECT_EQ(a, kvbe.Get("a"));
  // test the DeleteKey() functions
  kvbe.DeleteKey("a");
  EXPECT_EQ(5, kvbe.size());
  kvbe.DeleteKey("b");
  EXPECT_EQ(4, kvbe.size());
  kvbe.DeleteKey("c");
  EXPECT_EQ(3, kvbe.size());
}

// test the Service Layer's Register, Chirp, Follow, Read, and Monitor functions
TEST(SL_backend, RegisterChirpFollowReadMonitor) {
  ServiceLayerBackEndTesting slbet;
  std::string str;
  // test the illegal usernames that cannot be registered
  str = slbet.RegisterUser("newest");
  EXPECT_EQ(str, "illegal");
  str = slbet.RegisterUser("keyCounterForDataMap");
  EXPECT_EQ(str, "illegal");
  str = slbet.RegisterUser("registered user");
  EXPECT_EQ(str, "illegal");
  // test a username that can be successfully registered
  str = slbet.RegisterUser("Name");
  EXPECT_EQ(str, "success");
  // test an attempt to register a pre-existing username
  str = slbet.RegisterUser("Name");
  EXPECT_EQ(str, "taken");
  // re-test a successful username registration
  str = slbet.RegisterUser("Name2");
  EXPECT_EQ(str, "success");
  // test the chirp function with an invalid parent id
  str = slbet.Chirp("Name", "first tweet", "doesnt exist");
  EXPECT_EQ(str, "parent id does not exist");
  // test the chirp function with a valid chirp without a parent id
  str = slbet.Chirp("Name", "first tweet", "");
  EXPECT_EQ(str, "success");
  // test the chirp function with a valid chirp with a parent id
  str = slbet.Chirp("Name", "second tweet", "chirp_by: Name 0");
  EXPECT_EQ(str, "success");
  // test the follow function with an invalid username to follow
  str = slbet.Follow("Name", "Name3");
  EXPECT_EQ(str, "user to follow does not exist");
  // test the follow function with a successful following
  str = slbet.Follow("Name", "Name2");
  EXPECT_EQ(str, "success");
  // test the read function for a single chirp
  std::vector<chirp::Chirp> v;
  v = slbet.Read("chirp_by: Name 1");
  EXPECT_EQ(1, v.size());
  // test the read function for a chirp thread with 2 chirps
  v = slbet.Read("chirp_by: Name 0");
  EXPECT_EQ(2, v.size());
  // add a chirp to the thread
  str = slbet.Chirp("Name", "third tweet", "chirp_by: Name 1");
  // test the read function for a chirp thread with 3 chirps
  v = slbet.Read("chirp_by: Name 0");
  EXPECT_EQ(3, v.size());
}
// test whether we can persistent tags and there related chirps into client
TEST(test, SimpleTags) {
  ServiceLayerBackEndTesting slbet;
  auto str = slbet.Chirp("Name", "first #1 tweet", "");
  EXPECT_EQ(str, "success");
  std::string tag = "#1";
  auto chirp = slbet.GetTagInfo(tag);
  EXPECT_EQ("Name", (*chirp).username());
  EXPECT_EQ("first #1 tweet", (*chirp).text());
  EXPECT_EQ("", (*chirp).parent_id());
  str = slbet.Chirp("Name", "second #1 tweet #2 # ", "");
  EXPECT_EQ(str, "success");
  tag = "#1";
  chirp = slbet.GetTagInfo(tag);
  EXPECT_EQ("Name", (*chirp).username());
  EXPECT_EQ("second #1 tweet #2 # ", (*chirp).text());
  EXPECT_EQ("", (*chirp).parent_id());
  tag = "#2";
  chirp = slbet.GetTagInfo(tag);
  EXPECT_EQ("Name", (*chirp).username());
  EXPECT_EQ("second #1 tweet #2 # ", (*chirp).text());
  EXPECT_EQ("", (*chirp).parent_id());
  tag = "# ";
  chirp = slbet.GetTagInfo(tag);
  EXPECT_EQ(std::nullopt, chirp);
}
TEST(test, stream) {
  ServiceLayerBackEndTesting service;
  std::vector<chirp::Chirp> buffer;
  chirp::StreamRequest stream_request;
  stream_request.set_hashtag("#1");
  chirp::StreamReply stream_reply;
  std::thread th1(unittest::AutoChirp, std::ref(service), "Adam", 100, 3);
  //  A modified version of monitor which will automatically exit
  //  if time passed 510 ms in here
  auto th2 = service.StreamBuffer(&stream_reply, buffer);
  service.OpenBuffer();
  service.SetNumStreamLoop(100);
  service.Stream(&stream_request, &stream_reply);
  // Set the value in promise
  // Wait for thread to join
  th1.join();
  th2.join();
  EXPECT_EQ(3, buffer.size());
  int i = 0;
  for (auto v : buffer) {
    EXPECT_EQ("Adam", v.username());
    auto textstr = "Hello World + #1 " + std::to_string(i);
    EXPECT_EQ(textstr, v.text());
    i++;
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
