#include "gtest/gtest.h"
#include <cstddef>
#include "kv_backend.cc"

TEST(KV_backend, PutGetDelete) {
	KeyValueBackEnd kvbe;
	std::string a = "a";
	std::string b = "b";
	std::string c = "c";
	kvbe.Put(a, a);
	EXPECT_EQ(1, kvbe.size());
	kvbe.Put(b, b);
	EXPECT_EQ(2, kvbe.size());
	kvbe.Put(c, c);
	EXPECT_EQ(3, kvbe.size());
	EXPECT_EQ("a", kvbe.Get("a")[0]);
	EXPECT_EQ("b", kvbe.Get("b")[0]);
	EXPECT_EQ("c", kvbe.Get("c")[0]);
	kvbe.DeleteKey("a");
	EXPECT_EQ(2, kvbe.size());
	kvbe.DeleteKey("b");
	EXPECT_EQ(1, kvbe.size());
	kvbe.DeleteKey("c");
	EXPECT_EQ(0, kvbe.size());
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}