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

TEST(CommentBlock, Indentation)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << IndentPush() << IndentPush();
	w << CommentBlock([](WriterBase& w) {
		w << "TODO:" << IndentPush() << NextLine();
		w << "[ ] Buy trigger button" << NextLine();
		w << "[ ] Restore order" << IndentPush() << NextLine();
		w << "[ ] Clean kitchen" << NextLine();
		w << "[ ] Take out trash" << NextLine();
		w << IndentPop() << "[ ] Arm nukes";
	});

	EXPECT_STREQ(R"(		/* TODO:
		 * 	[ ] Buy trigger button
		 * 	[ ] Restore order
		 * 		[ ] Clean kitchen
		 * 		[ ] Take out trash
		 * 	[ ] Arm nukes
		 */)", t.c_str());
}
