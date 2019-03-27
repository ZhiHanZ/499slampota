
#include <cstddef>
#include <string>
#include <vector>
#include "key_value_backend.h"
#include "service_layer_backend_testing.h"

#include <gtest/gtest.h>

TEST(KV_backend, PutGetDelete) {
  KeyValueBackEnd kvbe;
  std::vector<std::string> a;
  a.push_back("a");
  std::vector<std::string> b;
  b.push_back("b");
  std::vector<std::string> c;
  c.push_back("c");
  kvbe.Put("a", "a");
  EXPECT_EQ(4, kvbe.size());
  kvbe.Put("b", "b");
  EXPECT_EQ(5, kvbe.size());
  kvbe.Put("c", "c");
  EXPECT_EQ(6, kvbe.size());
  EXPECT_EQ(a, kvbe.Get("a"));
  EXPECT_EQ(b, kvbe.Get("b"));
  EXPECT_EQ(c, kvbe.Get("c"));
  kvbe.Put("a", "b");
  a.push_back("b");
  EXPECT_EQ(a, kvbe.Get("a"));
  kvbe.DeleteKey("a");
  EXPECT_EQ(5, kvbe.size());
  kvbe.DeleteKey("b");
  EXPECT_EQ(4, kvbe.size());
  kvbe.DeleteKey("c");
  EXPECT_EQ(3, kvbe.size());
}

TEST(SL_backend, RegisterChirpFollowReadMonitor) {
  ServiceLayerBackEndTesting slbet;
  std::string str;
  str = slbet.RegisterUser("newest");
  EXPECT_EQ(str, "illegal");
  std::cout << "43" << std::endl;
  str = slbet.RegisterUser("keyCounterForDataMap");
  EXPECT_EQ(str, "illegal");
  std::cout << "44" << std::endl;
  str = slbet.RegisterUser("registered user");
  EXPECT_EQ(str, "illegal");
  std::cout << "45" << std::endl;
  str = slbet.RegisterUser("Name");
  EXPECT_EQ(str, "success");
  std::cout << "46" << std::endl;
  str = slbet.RegisterUser("Name");
  EXPECT_EQ(str, "taken");
  std::cout << "47" << std::endl;
  str = slbet.RegisterUser("Name2");
  EXPECT_EQ(str, "success");
  std::cout << "48" << std::endl;
  str = slbet.Chirp("Name", "first tweet", "doesnt exist");
  EXPECT_EQ(str, "parent id does not exist");
  std::cout << "49" << std::endl;
  str = slbet.Chirp("Name", "first tweet", "");
  EXPECT_EQ(str, "success");
  std::cout << "50" << std::endl;
  str = slbet.Chirp("Name", "second tweet", "chirp_by: Name 0");
  EXPECT_EQ(str, "success");
  std::cout << "51" << std::endl;
  str = slbet.Follow("Name", "Name3");
  EXPECT_EQ(str, "user to follow does not exist");
  std::cout << "52" << std::endl;
  str = slbet.Follow("Name", "Name2");
  EXPECT_EQ(str, "success");
  std::cout << "53" << std::endl;
  std::vector<chirp::Chirp> v;
  v = slbet.Read("chirp_by: Name 0");
  EXPECT_EQ(2, v.size());
  std::cout << "54" << std::endl;
  // monitor???
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}