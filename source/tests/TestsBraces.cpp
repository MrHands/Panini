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

TEST(Braces, BreakingStyleInheritEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](Writer&) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, BreakingStyleInheritRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](Writer& w) {
		w << "var i = 0;" << NextLine();
		w << "doSomething(i);" << NextLine();
		w << NextLine();
		w << "next->Call([]()" << Braces([](Writer& w) {
			w << "// result" << NextLine();
		}) << ");" << NextLine();
	});

	EXPECT_STREQ(R"({
	var i = 0;
	doSomething(i);

	next->Call([]()
	{
		// result
	});
})", t.c_str());
}

TEST(Braces, BreakingStyleAttachEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](Writer&) {
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, BreakingStyleAttachRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "std::sort(cards.begin(), cards.end(), [](const Card& left, const Card& right) " << Braces([](Writer& w) {
		w << "return left.face < right.face;" << NextLine();
	}, BraceBreakingStyle::Attach) << ");";

	EXPECT_STREQ(R"(std::sort(cards.begin(), cards.end(), [](const Card& left, const Card& right) {
	return left.face < right.face;
});)", t.c_str());
}

TEST(Braces, BreakingStyleAllmanEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](Writer& w) {
		(void)w;
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, BreakingStyleAllmanRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "void reportPictures(int32_t spiderManCount)" << Braces([](Writer& w) {
		w << "transferMoney(spiderManCount * 1000);" << NextLine();
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"(void reportPictures(int32_t spiderManCount)
{
	transferMoney(spiderManCount * 1000);
})", t.c_str());
}

TEST(Braces, BreakingStyleWhitesmithsEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](Writer&) {
	}, BraceBreakingStyle::Whitesmiths);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, BreakingStyleWhitesmithsRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "std::vector<Movie> getMoviesToWatch()" << Braces([](Writer& w) {
		w << "return std::vector<Movie>(" << Braces([](Writer& w) {
			w << R"({ "John Wick" }, { "John Wick 2" }, { "John Wick 3" }, { "John Wick 4" })" << NextLine();
		}) << ");" << NextLine();
	}, BraceBreakingStyle::Whitesmiths);

	EXPECT_STREQ(R"(std::vector<Movie> getMoviesToWatch()
	{
	return std::vector<Movie>(
	{
		{ "John Wick" }, { "John Wick 2" }, { "John Wick 3" }, { "John Wick 4" }
	});
	})", t.c_str());
}

TEST(Braces, BracesOptionsInherit)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	BracesOptions o;
	o.chunkBraceOpen = "<<";
	o.chunkBraceClose = ">>";

	w << Braces([](Writer& w) {
		w << "ruleTheWorld();" << NextLine();
	}, o);

	EXPECT_STREQ(R"(<<
	ruleTheWorld();
>>)", t.c_str());
}

TEST(Braces, BracesOptionsAttach)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	BracesOptions o;
	o.breakingStyle = panini::BraceBreakingStyle::Attach;
	o.chunkBraceOpen = "(";
	o.chunkBraceClose = ")";

	w << "constructor" << Braces([](Writer& w) {
		w << "args" << NextLine();
	}, o);

	EXPECT_STREQ(R"(constructor(
	args
))", t.c_str());
}

TEST(Braces, BracesOptionsAllman)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	BracesOptions o;
	o.breakingStyle = panini::BraceBreakingStyle::Allman;
	o.chunkBraceOpen = "<!--";
	o.chunkBraceClose = "-->";

	w << "XML string:" << Braces([](Writer& w) {
		w << "stable" << NextLine();
	}, o);

	EXPECT_STREQ(R"(XML string:
<!--
	stable
-->)", t.c_str());
}

TEST(Braces, BracesOptionsWhitesmiths)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	BracesOptions o;
	o.breakingStyle = panini::BraceBreakingStyle::Whitesmiths;
	o.chunkBraceOpen = "1.";
	o.chunkBraceClose = "2.";

	w << "first of all" << Braces([](Writer& w) {
		w << "yes" << NextLine();
		w << "but no" << NextLine();
	}, o);

	EXPECT_STREQ(R"(first of all
	1.
	yes
	but no
	2.)", t.c_str());
}

TEST(Braces, InheritConfigAttach)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i) " << Braces([](Writer& w) {
		w << "try " << Braces([](Writer& w) {
			w << "do " << Braces([](Writer& w) {
				w << "switch (i) " << Braces([](Writer& w) {
					w << "case 1: " << Braces([](Writer& w) {
						w << "foobar();" << NextLine();
						w << "break;" << NextLine();
					}) << NextLine();
				}) << NextLine();
			}) << NextLine();
		}) << NextLine();
	});

	EXPECT_STREQ(R"(bool baz(int i) {
	try {
		do {
			switch (i) {
				case 1: {
					foobar();
					break;
				}
			}
		}
	}
})", t.c_str());
}

TEST(Braces, InheritConfigAllman)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Allman;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i)" << Braces([](Writer& w) {
		w << "try" << Braces([](Writer& w) {
			w << "do" << Braces([](Writer& w) {
				w << "switch (i)" << Braces([](Writer& w) {
					w << "case 1:" << Braces([](Writer& w) {
						w << "foobar();" << NextLine();
						w << "break;" << NextLine();
					}) << NextLine();
				}) << NextLine();
			}) << NextLine();
		}) << NextLine();
	});

	EXPECT_STREQ(R"(bool baz(int i)
{
	try
	{
		do
		{
			switch (i)
			{
				case 1:
				{
					foobar();
					break;
				}
			}
		}
	}
})", t.c_str());
}

TEST(Braces, InheritConfigWhitesmiths)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Whitesmiths;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i)" << Braces([](Writer& w) {
		w << "try" << Braces([](Writer& w) {
			w << "do" << Braces([](Writer& w) {
				w << "switch (i)" << Braces([](Writer& w) {
					w << "case 1:" << Braces([](Writer& w) {
						w << "foobar();" << NextLine();
						w << "break;" << NextLine();
					}) << NextLine();
				}) << NextLine();
			}) << NextLine();
		}) << NextLine();
	});

	EXPECT_STREQ(R"(bool baz(int i)
	{
	try
		{
		do
			{
			switch (i)
				{
				case 1:
					{
					foobar();
					break;
					}
				}
			}
		}
	})", t.c_str());
}
