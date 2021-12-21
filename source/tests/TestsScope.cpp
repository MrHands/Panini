#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(Scope, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope([](WriterBase& w) {
		w << "var i = 0;" << NextLine();
		w << "doSomething(i);" << NextLine();
	});

	EXPECT_STREQ(R"({
	var i = 0;
	doSomething(i);
})", t.c_str());
}
