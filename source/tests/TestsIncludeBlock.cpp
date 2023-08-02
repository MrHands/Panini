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

TEST(IncludeBlock, Empty)
{
	using namespace panini;

	IncludeSet s;

	std::string t;
	StringWriter w(t);

	w << IncludeBlock(s);

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(IncludeBlock, Entries)
{
	using namespace panini;

	IncludeSet s;
	s.Add("game/systems/Audio.h");
	s.Add("game/Physics.h");
	s.Add("game/systems/Particles.h");
	s.Add("stdio.h", IncludeStyle::AngularBrackets);

	std::string t;
	StringWriter w(t);

	w << IncludeBlock(s);

	EXPECT_STREQ(R"(#include <stdio.h>
#include "game/systems/Audio.h"
#include "game/systems/Particles.h"
#include "game/Physics.h")", t.c_str());
}
