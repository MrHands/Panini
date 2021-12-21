#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(StringWriter, WriteChunk)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "What an intriguing device!";

	EXPECT_STREQ("What an intriguing device!", t.c_str());
}
