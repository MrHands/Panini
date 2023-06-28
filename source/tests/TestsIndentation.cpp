/*
	MIT No Attribution

	Copyright 2021-2022 Mr. Hands

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

TEST(Indentation, EmptyLine)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "// generated by" << IndentPush() << NextLine();
	w << NextLine();
	w << "// some really cool people";

	EXPECT_STREQ(R"(// generated by

	// some really cool people)", t.c_str());
}

TEST(Indentation, Config)
{
	using namespace panini;

	StringWriterConfig c;
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
