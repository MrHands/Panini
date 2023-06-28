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

TEST(Scope, BreakingStyleInheritEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](Writer&) {
	});

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Scope, BreakingStyleInheritRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("bool isUserAdmin(const char* username)", [](Writer& writer) {
		writer << "return true;" << NextLine();
	});

	EXPECT_STREQ(R"(bool isUserAdmin(const char* username)
{
	return true;
})", t.c_str());
}

TEST(Scope, BreakingStyleAttachEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](Writer&) {
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Scope, BreakingStyleAttachRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("namespace panini", [](Writer& writer) {
		writer << "struct Config;" << NextLine();
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"(namespace panini {
	struct Config;
})", t.c_str());
}

TEST(Scope, BreakingStyleAllmanEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](Writer&) {
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Scope, BreakingStyleAllmanRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("dipInAndOut()", [](Writer& writer) {
		writer << "charge(EPremium);" << NextLine();
	}, BraceBreakingStyle::Allman);

	EXPECT_STREQ(R"(dipInAndOut()
{
	charge(EPremium);
})", t.c_str());
}

TEST(Scope, BreakingStyleWhitesmithsEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](Writer&) {
	}, BraceBreakingStyle::Whitesmiths);

	EXPECT_STREQ(R"({
})", t.c_str());
}

TEST(Scope, BreakingStyleWhitesmithsRegular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("cooking()", [](Writer& writer) {
		writer << "scale(with_ingredients);" << NextLine();
	}, BraceBreakingStyle::Whitesmiths);

	EXPECT_STREQ(R"(cooking()
	{
	scale(with_ingredients);
	})", t.c_str());
}

TEST(Scope, ScopeOptionsInherit)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	ScopeOptions o;
	o.chunkBraceOpen = "<<";
	o.chunkBraceClose = ">>";

	w << Scope("strawberry", [](Writer& w) {
		w << "franchises;" << NextLine();
	}, o);

	EXPECT_STREQ(R"(strawberry
<<
	franchises;
>>)", t.c_str());
}

TEST(Scope, ScopeOptionsAttach)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	ScopeOptions o;
	o.breakingStyle = BraceBreakingStyle::Attach;
	o.chunkBraceOpen = "{{";
	o.chunkBraceClose = "}}";

	w << Scope("steak", [](Writer& w) {
		w << "laborCosts();" << NextLine();
	}, o);

	EXPECT_STREQ(R"(steak {{
	laborCosts();
}})", t.c_str());
}

TEST(Scope, ScopeOptionsWhitesmiths)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	ScopeOptions o;
	o.breakingStyle = BraceBreakingStyle::Whitesmiths;
	o.chunkBraceOpen = "d";
	o.chunkBraceClose = "b";

	w << Scope("WhatAreYou", [](Writer& w) {
		w << "LovingSandwich" << NextLine();
	}, o);

	EXPECT_STREQ(R"(WhatAreYou
	d
	LovingSandwich
	b)", t.c_str());
}

TEST(Scope, InheritConfigAttach)
{
	using namespace panini;

	Config c;
	c.braceBreakingStyle = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << Scope("struct Public", [](Writer& writer) {
		writer << Scope("Public()", [](Writer& writer) {
			writer << "m_private = true;" << NextLine();
		}) << NextLine();
	});

	EXPECT_STREQ(R"(struct Public {
	Public() {
		m_private = true;
	}
})", t.c_str());
}
