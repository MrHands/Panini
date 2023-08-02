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

TEST(FileWriter, Write)
{
	using namespace panini;

	FileWriterConfig c;
	c.targetPath = "file_out.txt";

	FileWriter w(c);
	w << "HACKING";
	w.Commit();

	std::ifstream f(c.targetPath);
	EXPECT_TRUE(f.is_open());
	std::stringstream ss;
	ss << f.rdbuf();

	EXPECT_STREQ("HACKING", ss.str().c_str());
}

TEST(FileWriter, PreventDoubleCommit)
{
	using namespace panini;

	FileWriterConfig c;
	c.targetPath = "file_double_commit.txt";

	FileWriter w(c);
	w << "Wherever you go, whatever you do";
	EXPECT_TRUE(w.IsChanged());
	EXPECT_TRUE(w.Commit());

	EXPECT_FALSE(w.IsChanged());
	EXPECT_FALSE(w.Commit());
}

TEST(FileWriter, ScopedCommit)
{
	using namespace panini;

	FileWriterConfig c;
	c.targetPath = "file_scoped_commit.txt";

	{
		FileWriter w(c);
		w << "Even if you snore";
	}

	std::ifstream f(c.targetPath);
	EXPECT_TRUE(f.is_open());
	std::stringstream ss;
	ss << f.rdbuf();

	EXPECT_STREQ("Even if you snore", ss.str().c_str());
}

TEST(FileWriter, DeprecatedConstructor)
{
	using namespace panini;

	std::filesystem::path fp = "file_deprecated.txt";

	FileWriter w(fp);
	w << "This seems like a bad idea.";
	w.Commit();

	std::ifstream f(fp);
	EXPECT_TRUE(f.is_open());
	std::stringstream ss;
	ss << f.rdbuf();

	EXPECT_STREQ("This seems like a bad idea.", ss.str().c_str());
}
