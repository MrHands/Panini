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

TEST(FeatureFlag, Empty)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(false, "", [](Writer&) {});

	EXPECT_STREQ(R"()", s.c_str());
}

TEST(FeatureFlag, ConditionTrue)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(true, "should_push", [](Writer& writer) {
		writer << "PushTheButton();" << NextLine();
	});

	EXPECT_STREQ(R"(// should_push
PushTheButton();
// should_push)", s.c_str());
}

TEST(FeatureFlag, ConditionTrueNoContext)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(true, "", [](Writer& writer) {
		writer << "DanceAwayTheNight();";
	});

	EXPECT_STREQ(R"(DanceAwayTheNight();)", s.c_str());
}

TEST(FeatureFlag, ConditionFalse)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(false, "use-wall-color", [](Writer& writer) {
		writer << "Paint(wall_color);";
	});

	EXPECT_STREQ(R"()", s.c_str());
}

TEST(FeatureFlag, ConditionTrueWithElse)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(true, "enable_dating", [](Writer& writer) {
		writer << "rock->Date();" << NextLine();
	}, [](Writer& writer) {
		writer << "rock->Throw();" << NextLine();
	});

	EXPECT_STREQ(R"(// enable_dating
rock->Date();
// enable_dating)", s.c_str());
}

TEST(FeatureFlag, ConditionFalseWithElse)
{
	using namespace panini;

	std::string s;
	StringWriter w(s);

	w << FeatureFlag(false, "paint-that-wall", [](Writer& writer) {
		writer << "Paint(wall_color);";
	}, [](Writer& writer) {
		writer << "PaintTheRoom();";
	});

	EXPECT_STREQ(R"(PaintTheRoom();)", s.c_str());
}
