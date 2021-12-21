#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(FileWriter, Write)
{
	using namespace panini;

	FileWriter w("out.txt");
	w << "HACKING";
	w.Close();

	std::string s;

	std::ifstream f("out.txt");
	EXPECT_TRUE(f.is_open());
	f >> s;

	EXPECT_STREQ("HACKING", s.c_str());
}
