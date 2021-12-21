#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(StringWriter, Chunk)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "What an intriguing device!";

	EXPECT_STREQ("What an intriguing device!", t.c_str());
}

TEST(StringWriter, NewLine)
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

TEST(StringWriter, NewLineWindows)
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
