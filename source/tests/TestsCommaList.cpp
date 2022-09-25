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

namespace panini
{

	struct CommaListOptions
	{
		std::string separatorBegin = "";
		std::string separatorEnd = ", ";
		bool addNewLine = false;
	};

	template <typename TIterator>
	class CommaList
		: public CommandBase
	{

	public:
		using TUnderlying = typename std::iterator_traits<TIterator>::value_type;

		template <typename TSource>
		static std::string DefaultTransform(TSource& source, size_t index)
		{
			(void)index;

			return std::to_string(source);
		}

		template <>
		static std::string DefaultTransform(std::string& source, size_t index)
		{
			(void)index;

			return source;
		}

		inline CommaList(
			TIterator begin,
			TIterator end,
			const CommaListOptions& options = {},
			std::function<std::string(TUnderlying&, size_t)> transform = DefaultTransform<TUnderlying>)
			: m_begin(begin)
			, m_end(end)
			, m_options(options)
			, m_transform(transform)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			size_t index = 0;
			for (TIterator item = m_begin; item != m_end; ++item)
			{
				if (index++ > 0)
				{
					writer << m_options.separatorEnd;
				}

				writer
					<< m_options.separatorBegin
					<< m_transform(*item, index);
			}
		}

	private:
		TIterator m_begin;
		TIterator m_end;
		CommaListOptions m_options;
		std::function<std::string(TUnderlying&, size_t)> m_transform;

	};

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

TEST(CommaList, VectorOfInts)
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

TEST(CommaList, Transform)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	std::vector<int32_t> s = {
		6, 12, 24, 48
	};

	w << CommaList(s.begin(), s.end(), {}, [](int32_t& it, size_t index) {
		return std::string{ "[ " } + std::to_string(100 + it) + " ]";
	});

	EXPECT_STREQ(R"([ 106 ], [ 112 ], [ 124 ], [ 148 ])", t.c_str());
}
