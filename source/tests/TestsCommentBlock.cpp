#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(CommentBlock, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentBlock([](WriterBase& w) {
		w << "In the beginning there was only darkness." << NextLine();
		w << "But then there was Disco.";
	});

	EXPECT_STREQ(R"(/* In the beginning there was only darkness.
 * But then there was Disco.
 */)", t.c_str());
}

TEST(CommentBlock, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentBlock([](WriterBase& w) {
	});

	EXPECT_STREQ(R"(/* 
 */)", t.c_str());
}
