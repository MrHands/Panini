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
		iterators, as long as the type implements the interface required
		for `std::iterator_traits` to derive the underlying type.

		If the underlying type for the iterator cannot be derived implicitly,
		you can use the TUnderlying parameter to set the type explicitly.

		Using the CommaListOptions struct, you can specify the separator chunk
		that should come before each item and the one that should come after
		the first item, which is ", " by default. It's also possible to add a
		new line after each item, which is disabled by default.

		Finally, you can add a transform function to the command, which will be
		called once for each item in the list and transforms the item to
		a string.

		Example:

		\code{.cpp}
			writer << Scope("enum Vehicles", [](WriterBase& writer) {
				CommaListOptions options;
				options.chunkEndSeparator = ",";
				options.addNewLines = true;

				writer << CommaList(myEnums.begin(), myEnums.end(), options) << NextLine();
			}) << ";";
		\endcode

		Output:

		\code{.cpp}
			enum Vehicles
			{
				DUCK_CAR,
				DUCK_PLANE,
				DUCK_MARINE
			};
		\endcode
	*/

	template <
		class TIterator,
		class TUnderlying = typename std::iterator_traits<TIterator>::value_type
	>
	class CommaList
		: public CommandBase
	{

	public:
		/*!
			\brief Default transform function for the command.

			Each iterated item must pass through a function that "transforms"
			it to an `std::string`. The default implementation calls
			`std::to_string`, which will handle most standard types.

			\param source   Value being processed.
			\param index    Index of the value in the list.

			\return Source value represented as a string.
		*/
		template <typename TSource>
		static std::string DefaultTransform(const TSource& source, size_t index)
		{
			(void)index;

			return std::to_string(source);
		}

		/*!
			Specialization for transforming string items that passes the input
			through unchanged.
		*/
		template <>
		static std::string DefaultTransform(const std::string& source, size_t index)
		{
			(void)index;

			return source;
		}

		/*!
			Construct a CommaList from a begin and end iterator.

			\param begin       Starting point for iteration.
			\param end         End point for iteration.
			\param options     Additional options for the command.
			\param transform   Transforms each iterated to a string.
		*/
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
				if (index > 0)
				{
					writer << m_options.chunkEndSeparator;

					if (m_options.addNewLines)
					{
						writer << NextLine();
					}
				}

				writer
					<< m_options.chunkBeginSeparator
					<< m_transform(*item, index);

				index++;
			}
		}

	private:
		TIterator m_begin;
		TIterator m_end;
		CommaListOptions m_options;
		std::function<std::string(const TUnderlying&, size_t)> m_transform;

	};

};
