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
	});

	EXPECT_STREQ(R"({
	var i = 0;
	doSomething(i);
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
