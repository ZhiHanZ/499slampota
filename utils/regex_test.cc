#include "regex.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using std::string;
using std::vector;
using utils::HashParser;

// test basic cases in HashParser where should be no hashtags
TEST(test, init) {
  auto null_str{""};
  auto invalid_str{"# "};
  auto null_vec = HashParser(null_str);
  ASSERT_EQ(0, null_vec.size());
  auto invalid_vec = HashParser(invalid_str);
  ASSERT_EQ(0, invalid_vec.size());
}
// test whether we can separate tests precisely in the begin and end
TEST(test, simple) {
  auto simple_str1{"Hello #World"};
  auto simple_str2{"#Hello World"};
  auto vec1 = HashParser(simple_str1);
  auto vec2 = HashParser(simple_str2);
  ASSERT_EQ(1, vec1.size());
  ASSERT_EQ("#World", vec1[0]);
  ASSERT_EQ(1, vec1.size());
  ASSERT_EQ("#Hello", vec2[0]);
}
// test several tricky things like blank #, hashtags in the middle
// and a huge hashtag;
TEST(test, tricky) {
  auto simple_str3{"#test I have #1 #2 # 3 #4 tests #&^%?"};
  auto vec1 = HashParser(simple_str3);
  ASSERT_EQ(5, vec1.size());
  ASSERT_EQ("#test", vec1[0]);
  ASSERT_EQ("#1", vec1[1]);
  ASSERT_EQ("#2", vec1[2]);
  ASSERT_EQ("#4", vec1[3]);
  ASSERT_EQ("#&^%?", vec1[4]);
  auto simple_str4{"#choiqjdoij1eGIWBNOJCIUA#$%^&*(?><m:::''''}{}[[|||]])"};
  auto vec2 = HashParser(simple_str4);
  ASSERT_EQ(1, vec2.size());
  ASSERT_EQ(simple_str4, vec2[0]);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
