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

#pragma once

#include "commands/CommandBase.hpp"
#include "data/CommaListOptions.hpp"

namespace panini
{

	/*!
		\brief Outputs a list of items, comma-separated by default.

		The CommaList command makes it easy to print a list of items that
		should be separated after the first item, e.g. function parameters.

		The command works with iterators of any STL container and even custom
		iterators, as long as the 

		Using the /ref CommaListOptions struct, you can specify the separator
		chunk that should come before each item and the one that should come
		after the first item, which is ", " by default. It's also possible to
		add a new line after each item, which is disabled by default.

		Example:

		\code{.cpp}
			writer << CommaList(myEnu
		\endcode
	*/

	template <typename TIterator>
	class CommaList
		: public CommandBase
	{

	public:
		using TUnderlying = typename std::iterator_traits<TIterator>::value_type;

		template <typename TSource>
		static std::string DefaultTransform(const TSource& source, size_t index)
		{
			(void)index;

			return std::to_string(source);
		}

		template <>
		static std::string DefaultTransform(const std::string& source, size_t index)
		{
			(void)index;

			return source;
		}

		inline explicit CommaList(
			TIterator begin,
			TIterator end,
			const CommaListOptions& options = {},
			std::function<std::string(const TUnderlying&, size_t)> transform = DefaultTransform<TUnderlying>)
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
					if (m_options.addNewLines)
					{
						writer << NextLine();
					}

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
		std::function<std::string(const TUnderlying&, size_t)> m_transform;

	};

};
