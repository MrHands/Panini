#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(Include, DoubleQuotesRegular)
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

TEST(Include, SingleQuotesRegular)
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

TEST(Include, AngularBracketsRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("stdint.h", IncludeStyle::AngularBrackets) << Include("stdio.h", IncludeStyle::AngularBrackets);

	EXPECT_STREQ(R"(#include <stdint.h>#include <stdio.h>)", t.c_str());
}

TEST(Include, AngularBracketsEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Include("", IncludeStyle::AngularBrackets);

	EXPECT_STREQ(R"(#include <>)", t.c_str());
}
