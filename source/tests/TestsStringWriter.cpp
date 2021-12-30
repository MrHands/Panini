#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(StringWriter, Write)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "What an intriguing device!";

	EXPECT_STREQ("What an intriguing device!", t.c_str());
}

TEST(Braces, ConfigInheritIsNotAllowed)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Inherit;
	std::string t;
	StringWriter w(t, c);

	EXPECT_EQ(BraceBreakingStyle::Allman, w.GetBraceBreakingStyle());
}

TEST(StringWriter, NewLine)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << NextLine();

	EXPECT_TRUE(w.IsOnNewLine());
}
