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

TEST(StringWriter, NewLine)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << NextLine();

	EXPECT_TRUE(w.IsOnNewLine());
}

TEST(StringWriter, Config)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Whitesmiths;
	c.includeStyle = IncludeStyle::SingleQuotes;
	c.chunkIndent = "  ";
	c.chunkNewLine = "\r\n";

	std::string t;
	StringWriter w(t, c);

	EXPECT_EQ(BraceBreakingStyle::Whitesmiths, w.GetBraceBreakingStyle());
	EXPECT_EQ(IncludeStyle::SingleQuotes, w.GetIncludeStyle());
}

TEST(StringWriter, InheritIsNotAllowed)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Inherit;
	c.includeStyle = IncludeStyle::Inherit;

	std::string t;
	StringWriter w(t, c);

	EXPECT_EQ(BraceBreakingStyle::Allman, w.GetBraceBreakingStyle());
	EXPECT_EQ(IncludeStyle::DoubleQuotes, w.GetIncludeStyle());
}
