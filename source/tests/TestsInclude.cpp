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

TEST(Include, Inherit)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("BikeRide.h");

	EXPECT_STREQ(R"(#include "BikeRide.h")", t.c_str());
}

TEST(Include, InheritEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("");

	EXPECT_STREQ(R"(#include "")", t.c_str());
}

TEST(Include, DoubleQuotesConfig)
{
	using namespace panini;

	StringWriterConfig c;
	c.includeStyle = panini::IncludeStyle::DoubleQuotes;

	std::string t;
	StringWriter w(t);

	w << Include("waterSim.h");

	EXPECT_STREQ(R"(#include "waterSim.h")", t.c_str());
}

TEST(Include, DoubleQuotesOverride)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("game/design.h", IncludeStyle::DoubleQuotes);

	EXPECT_STREQ(R"(#include "game/design.h")", t.c_str());
}

TEST(Include, DoubleQuotesEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("", IncludeStyle::DoubleQuotes);

	EXPECT_STREQ(R"(#include "")", t.c_str());
}

TEST(Include, SingleQuotesConfig)
{
	using namespace panini;

	StringWriterConfig c;
	c.includeStyle = panini::IncludeStyle::SingleQuotes;

	std::string t;
	StringWriter w(t, c);

	w << Include("components/Button");

	EXPECT_STREQ(R"(#include 'components/Button')", t.c_str());
}

TEST(Include, SingleQuotesOverride)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("BringIt.inl", IncludeStyle::SingleQuotes);

	EXPECT_STREQ(R"(#include 'BringIt.inl')", t.c_str());
}

TEST(Include, SingleQuotesEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("", IncludeStyle::SingleQuotes);

	EXPECT_STREQ(R"(#include '')", t.c_str());
}

TEST(Include, AngularBracketsConfig)
{
	using namespace panini;

	StringWriterConfig c;
	c.includeStyle = panini::IncludeStyle::AngularBrackets;

	std::string t;
	StringWriter w(t, c);

	w << Include("stdint.h") << Include("stdio.h");

	EXPECT_STREQ(R"(#include <stdint.h>#include <stdio.h>)", t.c_str());
}

TEST(Include, AngularBracketsOverride)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("future.h", IncludeStyle::AngularBrackets);

	EXPECT_STREQ(R"(#include <future.h>)", t.c_str());
}

TEST(Include, AngularBracketsEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("", IncludeStyle::AngularBrackets);

	EXPECT_STREQ(R"(#include <>)", t.c_str());
}
