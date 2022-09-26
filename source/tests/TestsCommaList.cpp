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

TEST(CommaList, VectorOfStrings)
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

TEST(CommaList, SetOfStrings)
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

TEST(CommaList, VectorOfIntegers)
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

TEST(CommaList, SeparatorBegin)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Taxi", "Cab", "Service"
	};

	CommaListOptions o;
	o.separatorBegin = " -> ";
	o.separatorEnd = "";

	// explicit specialization is a workaround for an internal compiler error
	// when compiling with VS 2017
	using TCommaList = CommaList<std::vector<std::string>::const_iterator>;
	w << TCommaList(s.begin(), s.end(), o, TCommaList::DefaultTransform<std::string>);

	EXPECT_STREQ(R"( -> Taxi -> Cab -> Service)", t.c_str());
}

TEST(CommaList, SeparatorEnd)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<std::string> s = {
		"Blinded", "By", "The", "Lights"
	};

	CommaListOptions o;
	o.separatorBegin = "";
	o.separatorEnd = " and then ";

	// explicit specialization is a workaround for an internal compiler error
	// when compiling with VS 2017
	using TCommaList = CommaList<std::vector<std::string>::const_iterator>;
	w << TCommaList(s.begin(), s.end(), o, TCommaList::DefaultTransform<std::string>);

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

	// explicit specialization is a workaround for an internal compiler error
	// when compiling with VS 2017
	using TCommaList = CommaList<std::vector<std::string>::const_iterator>;
	w << TCommaList(s.begin(), s.end(), o, TCommaList::DefaultTransform<std::string>);

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

	// explicit specialization is a workaround for an internal compiler error
	// when compiling with VS 2017
	using TCommaList = CommaList<std::vector<std::string>::const_iterator>;
	w << TCommaList(s.begin(), s.end(), o, TCommaList::DefaultTransform<std::string>);

	EXPECT_STREQ(R"(Next)", t.c_str());
}

TEST(CommaList, Transform)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<int32_t> s = {
		6, 12, 24, 48
	};

	w << CommaList(s.begin(), s.end(), {}, [](const int32_t& it, size_t index) {
		return std::string{ "[ " } + std::to_string(100 + it) + " ]";
	});

	EXPECT_STREQ(R"([ 106 ], [ 112 ], [ 124 ], [ 148 ])", t.c_str());
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

	w << CommaList<CustomIterator>(ib, ie);

	EXPECT_STREQ(R"(North, South, East, West)", t.c_str());
}
