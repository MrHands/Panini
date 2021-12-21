#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(NewLine, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "[ ] Eggs" << NextLine();
	w << "[ ] Milk" << NextLine();
	w << "[ ] Bananas";

	EXPECT_STREQ(R"([ ] Eggs
[ ] Milk
[ ] Bananas)", t.c_str());
}

TEST(NewLine, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << NextLine();

	EXPECT_STREQ("\n", t.c_str());
}

TEST(NewLine, OnWindows)
{
	using namespace panini;

	Config c;
	c.chunkNewLine = "\r\n";
	std::string t;
	StringWriter w(t, c);

	w << "West ->" << NextLine();
	w << "South |";

	EXPECT_STREQ("West ->\r\nSouth |", t.c_str());
}
