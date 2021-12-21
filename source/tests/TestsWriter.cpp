#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(StringWriter, Chunk)
{
	using namespace panini;

	Config c;
	std::string t;
	StringWriter w(t, c);

	w << "What an intriguing device!";

	EXPECT_STREQ("What an intriguing device!", t.c_str());
}
