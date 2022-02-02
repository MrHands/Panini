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

#include <filesystem>

TEST(CompareWriter, Write)
{
	using namespace panini;

	std::filesystem::remove("compare_write.txt");

	CompareWriter w("compare_write.txt");
	w << "I'll catch you next time, Gadget!";
	EXPECT_TRUE(w.Commit());

	std::stringstream ss;

	std::ifstream f("compare_write.txt");
	EXPECT_TRUE(f.is_open());
	ss << f.rdbuf();

	EXPECT_STREQ("I'll catch you next time, Gadget!", ss.str().c_str());
}

TEST(CompareWriter, Exists)
{
	using namespace panini;

	std::ofstream p("compare_exists.txt");
	p << "TRUE TRUE FALSE";
	p.close();

	CompareWriter w("compare_exists.txt");
	w << "TRUE TRUE FALSE";
	EXPECT_TRUE(w.Commit());

	std::stringstream ss;

	std::ifstream f("compare_exists.txt");
	EXPECT_TRUE(f.is_open());
	ss << f.rdbuf();

	EXPECT_STREQ("TRUE TRUE FALSE", ss.str().c_str());
}

TEST(CompareWriter, Changed)
{
	using namespace panini;

	std::ofstream p("compare_changed.txt");
	p << "going to { the moon }";
	p.close();

	CompareWriter w("compare_changed.txt");
	w << "going to { mars }";
	EXPECT_TRUE(w.Commit());

	std::stringstream ss;

	std::ifstream f("compare_changed.txt");
	EXPECT_TRUE(f.is_open());
	ss << f.rdbuf();

	EXPECT_STREQ("going to { mars }", ss.str().c_str());
}
