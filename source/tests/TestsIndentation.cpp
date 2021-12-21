#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(Indentation, Regular)
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

TEST(Indentation, PushOnly)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPush();

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(Indentation, PopOnly)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPop();

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(Indentation, InTheMiddle)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "Let's get these" << IndentPush() << IndentPush() << IndentPush() << " values" << NextLine();
	w << "Shall we?";

	EXPECT_STREQ(R"(Let's get these values
			Shall we?)", t.c_str());
}

TEST(Indentation, Config)
{
	using namespace panini;

	Config c;
	c.chunkIndent = "  ";
	std::string t;
	StringWriter w(t, c);

	w << "var code = 0;" << IndentPush() << NextLine();
	w << "{" << IndentPush() << NextLine();
	w << "// looks great" << IndentPop() << NextLine();
	w << "}" << IndentPop() << NextLine();
	w << "var done = true;";

	EXPECT_STREQ(R"(var code = 0;
  {
    // looks great
  }
var done = true;)", t.c_str());
}