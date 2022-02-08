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

TEST(IncludeSet, Empty)
{
	using namespace panini;

	IncludeSet s;

	auto& e = s.GetEntries();

	EXPECT_EQ(0, e.size());
}

TEST(IncludeSet, StylePriorities)
{
	using namespace panini;

	IncludeSet s;
	s.AddInclude("vector1.h", IncludeStyle::SingleQuotes);
	s.AddInclude("vector2.h", IncludeStyle::DoubleQuotes);
	s.AddInclude("vector3.h", IncludeStyle::AngularBrackets);
	s.AddInclude("vector4.h", IncludeStyle::Inherit);

	auto& e = s.GetEntries();

	ASSERT_EQ(4, e.size());
	EXPECT_STREQ("vector1.h", e[0].path.string().c_str());
	EXPECT_STREQ("vector2.h", e[1].path.string().c_str());
	EXPECT_STREQ("vector3.h", e[2].path.string().c_str());
	EXPECT_STREQ("vector4.h", e[3].path.string().c_str());

	s.SortEntries(IncludeStyle::AngularBrackets);

	EXPECT_STREQ("vector3.h", e[0].path.string().c_str());
	EXPECT_STREQ("vector4.h", e[1].path.string().c_str());
	EXPECT_STREQ("vector2.h", e[2].path.string().c_str());
	EXPECT_STREQ("vector1.h", e[3].path.string().c_str());
}

TEST(IncludeSet, SortFoldersFirst)
{
	using namespace panini;

	IncludeSet s;
	s.AddInclude("Game/Design/Map.h");
	s.AddInclude("Game/Water.h");
	s.AddInclude("Game/Land/Impl.h");
	s.AddInclude("Game/Water/Fish.h");

	s.SortEntries(IncludeStyle::DoubleQuotes);

	auto& e = s.GetEntries();

	ASSERT_EQ(4, e.size());
	EXPECT_STREQ("Game/Design/Map.h", e[0].path.string().c_str());
	EXPECT_STREQ("Game/Land/Impl.h", e[1].path.string().c_str());
	EXPECT_STREQ("Game/Water/Fish.h", e[2].path.string().c_str());
	EXPECT_STREQ("Game/Water.h", e[3].path.string().c_str());
}

TEST(IncludeSet, SortWithInheritIsInvalid)
{
	using namespace panini;

	IncludeSet s;
	s.AddInclude("Water.h");
	s.AddInclude("Aquarium/Reef.h");
	s.AddInclude("Aquarium/Clownfish.h");

	s.SortEntries(IncludeStyle::Inherit);

	auto& e = s.GetEntries();

	ASSERT_EQ(3, e.size());
	EXPECT_STREQ("Water.h", e[0].path.string().c_str());
	EXPECT_STREQ("Aquarium/Reef.h", e[1].path.string().c_str());
	EXPECT_STREQ("Aquarium/Clownfish.h", e[2].path.string().c_str());
}
