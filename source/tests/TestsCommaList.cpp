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

#include <iterator>
#include <gtest/gtest.h>
#include <Panini.hpp>

class CustomIterator
{

public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::string;
	using pointer = value_type*;
	using reference = value_type&;

	CustomIterator(pointer source)
		: m_value(source)
	{
	}

	bool operator != (CustomIterator& other) const
	{
		return m_value != other.m_value;
	}

	reference operator*()
	{
		return *m_value;
	}

	CustomIterator& operator++()
	{
		m_value++;

		return *this;
	}

private:
	pointer m_value = nullptr;

};

TEST(CommaList, Vector)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"supposed", "to", "be", "somewhere"
	};

	w << CommaList(s.begin(), s.end());

	EXPECT_STREQ(R"(supposed, to, be, somewhere)", t.c_str());
}

TEST(CommaList, VectorEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {};

	w << CommaList(s.begin(), s.end());

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(CommaList, VectorReversed)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Muffins", "Delicious", "Sale", "For"
	};

	w << CommaList(s.rbegin(), s.rend());

	EXPECT_STREQ(R"(For, Sale, Delicious, Muffins)", t.c_str());
}

TEST(CommaList, Set)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::set<std::string> s = {
		"Apple", "Banana", "Cherry"
	};

	w << CommaList(s.begin(), s.end());

	EXPECT_STREQ(R"(Apple, Banana, Cherry)", t.c_str());
}

TEST(CommaList, BeginSeparatorSingleItem)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Crab"
	};

	CommaListOptions o;
	o.chunkBeginSeparator = "turn into ";
	o.chunkEndSeparator = "";

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(turn into Crab)", t.c_str());
}

TEST(CommaList, BeginSeparatorMultipleItems)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Taxi", "Cab", "Service"
	};

	CommaListOptions o;
	o.chunkBeginSeparator = " -> ";
	o.chunkEndSeparator = "";

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"( -> Taxi -> Cab -> Service)", t.c_str());
}

TEST(CommaList, EndSeparatorSingleItem)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Electricity"
	};

	CommaListOptions o;
	o.chunkBeginSeparator = "";
	o.chunkEndSeparator = ": [0]";

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(Electricity)", t.c_str());
}

TEST(CommaList, EndSeparatorMultipleItems)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Blinded", "By", "The", "Lights"
	};

	CommaListOptions o;
	o.chunkBeginSeparator = "";
	o.chunkEndSeparator = " and then ";

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(Blinded and then By and then The and then Lights)", t.c_str());
}

TEST(CommaList, AddNewLines)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"EFirst", "EThird", "ELast",
	};

	CommaListOptions o;
	o.addNewLines = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(EFirst, 
EThird, 
ELast)", t.c_str());
}

TEST(CommaList, AddNewLinesSingleItem)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Next"
	};

	CommaListOptions o;
	o.addNewLines = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(Next)", t.c_str());
}

TEST(CommaList, SkipFirstItemSingleItem)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Fish",
	};

	CommaListOptions o;
	o.chunkBeginSeparator = "->";
	o.skipFirstItemBeginSeparator = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(Fish)", t.c_str());
}

TEST(CommaList, SkipFirstItemMultipleItems)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"this.coin == other.coin",
		"this.stage == other.stage",
		"this.magic.compares(other.magic)",
	};

	CommaListOptions o;
	o.chunkBeginSeparator = "|| ";
	o.chunkEndSeparator = "";
	o.addNewLines = true;
	o.skipFirstItemBeginSeparator = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(this.coin == other.coin
|| this.stage == other.stage
|| this.magic.compares(other.magic))", t.c_str());
}

TEST(CommaList, SkipFirstItemEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {};

	CommaListOptions o;
	o.chunkBeginSeparator = " o ";
	o.skipFirstItemBeginSeparator = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(CommaList, KeepLastItemSingleItem)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"lunch"
	};

	CommaListOptions o;
	o.chunkEndSeparator = "<>";
	o.skipLastItemEndSeparator = false;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(lunch<>)", t.c_str());
}

TEST(CommaList, KeepLastItemMultipleItems)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Water",
		"Beer",
		"Whiskey"
	};

	CommaListOptions o;
	o.skipLastItemEndSeparator = false;
	o.addNewLines = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"(Water, 
Beer, 
Whiskey, )", t.c_str());
}

TEST(CommaList, KeepLastItemEmpty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {};

	CommaListOptions o;
	o.chunkEndSeparator = " [[[";
	o.skipLastItemEndSeparator = true;

	w << CommaList(s.begin(), s.end(), o);

	EXPECT_STREQ(R"()", t.c_str());
}

TEST(CommaList, Transform)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<int32_t> s = {
		6, 12, 24, 48
	};

	CommaListOptions o;

	w << CommaList(s.cbegin(), s.cend(), o, [](Writer& writer, const int32_t& it, size_t index) {
		(void)index;
		writer << "[ " << std::to_string(100 + it) << " ]";
	});

	EXPECT_STREQ(R"([ 106 ], [ 112 ], [ 124 ], [ 148 ])", t.c_str());
}

TEST(CommaList, TransformIntegers)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<int32_t> s = {
		1, 3, 5, 7, 11
	};

	w << CommaList(s.begin(), s.end());

	EXPECT_STREQ(R"(1, 3, 5, 7, 11)", t.c_str());
}

TEST(CommaList, TransformFirstItemOnly)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<float> s = {
		2.3f, 9.12f, 4.2f, 0.001f
	};

	CommaListOptions o;
	o.chunkEndSeparator = " + ";

	w << CommaList(s.begin(), s.end(), o, [](Writer& writer, const float& it, size_t index) {
		if (index == 0) {
			writer << "{" << std::to_string(it) << "}";

			return;
		}

		writer << std::to_string(it);
	});

	EXPECT_STREQ(R"({2.300000} + 9.120000 + 4.200000 + 0.001000)", t.c_str());
}

TEST(CommaList, TransformMap)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::map<std::string, std::string> s = {
		{ "Engines", "GO" },
		{ "Navigation", "GO" },
		{ "Landing", "HOLD" },
	};

	CommaListOptions o;

	w << CommaList(s.begin(), s.end(), o, [](Writer& writer, const std::pair<std::string, std::string>& it, size_t index) {
		(void)index;
		writer << it.first << ": " << it.second;
	});

	EXPECT_STREQ(R"(Engines: GO, Landing: HOLD, Navigation: GO)", t.c_str());
}

TEST(CommaList, Pointers)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::string s[] = {
		"Push",
		"Squeeze",
		"Bang",
		"Blow"
	};
	std::string* sb = &s[0];

	const size_t sl = sizeof(s) / sizeof(std::string);
	std::string* se = &s[sl];

	w << CommaList(sb, se);

	EXPECT_STREQ(R"(Push, Squeeze, Bang, Blow)", t.c_str());
}

TEST(CommaList, PointersWithTransform)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::string s[] = {
		"You",
		"Belong",
		"To"
	};
	std::string* sb = &s[0];

	const size_t sl = sizeof(s) / sizeof(std::string);
	std::string* se = &s[sl];

	CommaListOptions o;
	o.addNewLines = true;

	w << CommaList(sb, se, o, [](Writer& writer, const std::string& it, size_t index) {
		(void)index;
		writer << it;
	});

	EXPECT_STREQ(R"(You, 
Belong, 
To)", t.c_str());
}

TEST(CommaList, CustomIterator)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::string s[] = {
		"North",
		"South",
		"East",
		"West"
	};
	const size_t sl = sizeof(s) / sizeof(std::string);

	CustomIterator ib(&s[0]);
	CustomIterator ie(&s[sl]);

	w << CommaList(ib, ie);

	EXPECT_STREQ(R"(North, South, East, West)", t.c_str());
}

TEST(CommaList, CustomIteratorWithTransform)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::string s[] = {
		"GundamOne",
		"GundamZ",
	};
	const size_t sl = sizeof(s) / sizeof(std::string);

	CustomIterator ib(&s[0]);
	CustomIterator ie(&s[sl]);

	CommaListOptions o;

	w << CommaList(ib, ie, o, [](Writer& writer, const std::string& it, size_t index) {
		(void)index;
		writer << "[ " << it << " ]";
	});

	EXPECT_STREQ(R"([ GundamOne ], [ GundamZ ])", t.c_str());
}

TEST(CommaList, Example)
{
	using namespace panini;

	std::string t;
	StringWriter writer(t);

	writer << Scope("enum Vehicles", [](Writer& writer) {
		CommaListOptions options;
		options.chunkEndSeparator = ",";
		options.addNewLines = true;

		std::vector<std::string> myEnums = {
			"DUCK_CAR",
			"DUCK_PLANE",
			"DUCK_MARINE"
		};

		writer << CommaList(myEnums.begin(), myEnums.end(), options) << NextLine();
	}) << ";";

	EXPECT_STREQ(R"(enum Vehicles
{
	DUCK_CAR,
	DUCK_PLANE,
	DUCK_MARINE
};)", t.c_str());
}
