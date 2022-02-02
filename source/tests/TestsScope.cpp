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

TEST(Scope, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("bool isUserAdmin(const char* username)", [](WriterBase& writer) {
		writer << "return true;" << NextLine();
	});

	EXPECT_STREQ(R"(bool isUserAdmin(const char* username)
{
	return true;
})", t.c_str());
}

TEST(Scope, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](WriterBase& writer) {
		writer << "bool game_running = true;" << NextLine();
	});

	EXPECT_STREQ(R"({
	bool game_running = true;
})", t.c_str());
}

TEST(Scope, OverrideBracesAttach)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("namespace panini", [](WriterBase& writer) {
		writer << "struct Config;" << NextLine();
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"(namespace panini {
	struct Config;
})", t.c_str());
}

TEST(Scope, ConfigBracesAttach)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << Scope("struct Public", [](WriterBase& writer) {
		writer << "Public() = default;" << NextLine();
	});

	EXPECT_STREQ(R"(struct Public {
	Public() = default;
})", t.c_str());
}
