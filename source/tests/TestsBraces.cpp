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

	next->Call([](){
		// result
	});
})", t.c_str());
}

TEST(Braces, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Braces([](WriterBase& w) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Braces, Attach)
{
	using namespace panini;

	Config c;
	c.braces = panini::BraceBreakingStyle::Attach;
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

TEST(Braces, Allman)
{
	using namespace panini;

	Config c;
	c.braces = panini::BraceBreakingStyle::Allman;
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

TEST(Braces, Whitesmiths)
{
	using namespace panini;

	Config c;
	c.braces = panini::BraceBreakingStyle::Whitesmiths;
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
