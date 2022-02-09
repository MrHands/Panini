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

	w << CommentLine("");

	EXPECT_STREQ(R"(//)", t.c_str());
}

TEST(CommentLine, Doubled)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentLine("//pack your bags");

	EXPECT_STREQ(R"(// //pack your bags)", t.c_str());
}
