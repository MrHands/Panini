#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(Braces, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](WriterBase& w) {
		w << "var i = 0;" << NextLine();
		w << "doSomething(i);" << NextLine();
		w << NextLine();
		w << "next->Call([]()" << Braces([](WriterBase& w) {
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

TEST(Braces, ConfigInheritIsNotAllowed)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Inherit;
	std::string t;
	StringWriter w(t, c);

	EXPECT_EQ(BraceBreakingStyle::Allman, w.GetBraceStyle());
}

TEST(Braces, OverrideAttachRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "std::sort(cards.begin(), cards.end(), [](const Card& left, const Card& right) " << Braces([](WriterBase& w) {
		w << "return left.face < right.face;" << NextLine();
	}, BraceBreakingStyle::Attach) << ");";

	EXPECT_STREQ(R"(std::sort(cards.begin(), cards.end(), [](const Card& left, const Card& right) {
	return left.face < right.face;
});)", t.c_str());
}

TEST(Braces, OverrideAttachEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](WriterBase& w) {
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, ConfigAttachRegular)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i) " << Braces([](WriterBase& w) {
		w << "try " << Braces([](WriterBase& w) {
			w << "do " << Braces([](WriterBase& w) {
				w << "switch (i) " << Braces([](WriterBase& w) {
					w << "case 1: " << Braces([](WriterBase& w) {
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

TEST(Braces, ConfigAttachEmpty)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << Braces([](WriterBase& w) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, OverrideAllmanRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "void reportPictures(int32_t spiderManCount)" << Braces([](WriterBase& w) {
		w << "transferMoney(spiderManCount * 1000);" << NextLine();
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"(void reportPictures(int32_t spiderManCount)
{
	transferMoney(spiderManCount * 1000);
})", t.c_str());
}

TEST(Braces, OverrideAllmanEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](WriterBase& w) {
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, ConfigAllmanRegular)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Allman;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i)" << Braces([](WriterBase& w) {
		w << "try" << Braces([](WriterBase& w) {
			w << "do" << Braces([](WriterBase& w) {
				w << "switch (i)" << Braces([](WriterBase& w) {
					w << "case 1:" << Braces([](WriterBase& w) {
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

TEST(Braces, ConfigAllmanEmpty)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Allman;
	std::string t;
	StringWriter w(t, c);

	w << Braces([](WriterBase& w) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, OverrideWhitesmithsRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << "std::vector<Movie> getMoviesToWatch()" << Braces([](WriterBase& w) {
		w << "return std::vector<Movie>(" << Braces([](WriterBase& w) {
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

TEST(Braces, OverrideWhitesmithsEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](WriterBase& w) {
	}, BraceBreakingStyle::Whitesmiths);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, ConfigWhitesmithsRegular)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Whitesmiths;
	std::string t;
	StringWriter w(t, c);

	w << "bool baz(int i)" << Braces([](WriterBase& w) {
		w << "try" << Braces([](WriterBase& w) {
			w << "do" << Braces([](WriterBase& w) {
				w << "switch (i)" << Braces([](WriterBase& w) {
					w << "case 1:" << Braces([](WriterBase& w) {
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

TEST(Braces, ConfigWhitesmithsEmpty)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Whitesmiths;
	std::string t;
	StringWriter w(t, c);

	w << Braces([](WriterBase& w) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}
