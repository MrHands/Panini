#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(CommentLine, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentLine("be careful, might be hot");

	EXPECT_STREQ(R"(// be careful, might be hot)", t.c_str());
}

TEST(CommentLine, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentLine();

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(CommentLine, Doubled)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentLine("//pack your bags");

	EXPECT_STREQ(R"(// //pack your bags)", t.c_str());
}
