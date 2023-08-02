/*
	MIT No Attribution

	Copyright 2021-2023 Mr. Hands

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to
	deal in the Software without restriction, including without limitation the
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

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

	StringWriterConfig c;
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

	StringWriterConfig c;
	c.braceBreakingStyle = BraceBreakingStyle::Inherit;
	c.includeStyle = IncludeStyle::Inherit;

	std::string t;
	StringWriter w(t, c);

	EXPECT_EQ(BraceBreakingStyle::Allman, w.GetBraceBreakingStyle());
	EXPECT_EQ(IncludeStyle::DoubleQuotes, w.GetIncludeStyle());
}
