#include "gtest/gtest.h"
#include <cstddef>
#include "sl_backend.cc"
#include <iostream>


TEST(SL_backend, RUCFRM) {
	ServiceLayerBackEnd slbe;
	std::string a = "a";
	std::string b = "b";
	std::string c = "c";
	slbe.RegisterUser("Stephanie");
	slbe.Chirp("Stephanie", "my first chirp", "");
	EXPECT_EQ(slbe.Read("chirp_by Stephanie 0").size(), 1);
	slbe.Chirp("Stephanie", "thats cool", "chirp_by Stephanie 0");
	// EXPECT_EQ(slbe.Read("chirp_by Stephanie 0").size(), 1);
	// EXPECT_EQ("a", kvbe.Get("a"));
	// EXPECT_EQ("b", kvbe.Get("b"));
	// EXPECT_EQ("c", kvbe.Get("c"));
	// kvbe.DeleteKey("a");
	// EXPECT_EQ(2, kvbe.size());
	// kvbe.DeleteKey("b");
	// EXPECT_EQ(1, kvbe.size());
	// kvbe.DeleteKey("c");
	// EXPECT_EQ(0, kvbe.size());
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}