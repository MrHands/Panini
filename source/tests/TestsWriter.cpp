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

TEST(StringWriter, NewLine)
{
	using namespace panini;

	Config c;
	std::string t;
	StringWriter w(t, c);

	w << "[ ] Eggs" << NextLine();
	w << "[ ] Milk" << NextLine();
	w << "[ ] Bananas";

	EXPECT_STREQ(R"([ ] Eggs
[ ] Milk
[ ] Bananas)", t.c_str());
}

TEST(StringWriter, NewLineOnly)
{
	using namespace panini;

	Config c;
	std::string t;
	StringWriter w(t, c);

	w << NextLine();

	EXPECT_STREQ("\n", t.c_str());
}

TEST(StringWriter, NewLineOnWindows)
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

TEST(StringWriter, Indent)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPush() << "Drive down the road" << NextLine();
	w << IndentPop() << "Then turn around" << NextLine();
	w << IndentPush() << IndentPush() << "Bop it!" << IndentPush() << IndentPop() << "Twist it!";

	EXPECT_STREQ(R"(	Drive down the road
Then turn around
		Bop it!Twist it!)", t.c_str());
}

TEST(StringWriter, IndentPushOnly)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPush();

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(StringWriter, IndentPopOnly)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPop();

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(StringWriter, IndentInTheMiddle)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "Let's get these" << IndentPush() << IndentPush() << IndentPush() << " values" << NextLine();
	w << "Shall we?";

	EXPECT_STREQ(R"(Let's get these values
			Shall we?)", t.c_str());
}
