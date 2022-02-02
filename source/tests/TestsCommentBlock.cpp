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

TEST(CommentBlock, EmptyLines)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << CommentBlock([](WriterBase& w) {
		w << "First:" << NextLine();
		w << NextLine();
		w << "Third:";
	});

	EXPECT_STREQ(R"(/* First:
 *
 * Third:
 */)", t.c_str());
}
