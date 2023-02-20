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
#include "options/CommaListOptions.hpp"

#include <type_traits>

namespace panini
{

	/*!
		\brief Command for outputting a list of items, comma-separated by
		default.

		The CommaList command makes it easy to print a list of items that should
		be separated after the first item, e.g. function parameters.

		The command works with pointers of any type and iterators of any STL
		container. Custom iterators are supported as well, as long as the type
		implements the interface required for `std::iterator_traits` to derive
		the underlying type.

		Using the CommaListOptions struct, you can specify the separator chunk
		that should come before each item and the one that should come after the
		first item, which is ", " by default. It's also possible to add a new
		line after each item, which is disabled by default.

		Finally, you can add a transform function to the command, which will be
		called once for each item in the list and is used to transform your data
		before being passed to the writer.

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

	template <class TIterator>
	class CommaList
		: public CommandBase
	{

	public:
		/*!
			Underlying type as derived from TIterator, which should be either a
			pointer type or an iterator one.
		*/
		using TUnderlying = typename std::conditional<
			std::is_pointer_v<TIterator>,
			std::remove_pointer_t<TIterator>,
			typename std::iterator_traits<TIterator>::value_type
		>::type;

		/*!
			\brief Default transform function for the command.

			Each iterated item is passed through a function that "transforms" it
			to an `std::string` before passing it to the active writer.
			
			The default implementation calls `std::to_string`, which will handle
			most standard types.

			\param writer   Active writer.
			\param item     Value being processed.
			\param index    Index of the value in the list.

			\return Source value represented as a string.
		*/
		template <typename TItem>
		static void DefaultTransform(WriterBase& writer, const TItem& item, size_t index)
		{
			(void)index;

			writer << std::to_string(item);
		}

		/*!
			Specialization for transforming string items that passes the input
			through unchanged.
		*/
		template <>
		static void DefaultTransform(WriterBase& writer, const std::string& item, size_t index)
		{
			(void)index;

			writer << item;
		}

		/*!
			Construct a CommaList from a begin and end iterator.

			\param begin       Starting point for iteration.
			\param end         End point for iteration.
			\param options     Additional options for the command.
		*/
		inline explicit CommaList(
			TIterator begin,
			TIterator end,
			const CommaListOptions& options = {})
			: m_begin(begin)
			, m_end(end)
			, m_options(options)
			, m_transform(DefaultTransform<TUnderlying>)
		{
		}

		/*!
			Construct a CommaList and add a transform function.

			\param begin       Starting point for iteration.
			\param end         End point for iteration.
			\param options     Additional options for the command.
			\param transform   Transforms each iterated to a string.
		*/
		inline explicit CommaList(
			TIterator begin,
			TIterator end,
			const CommaListOptions& options,
			std::function<void(WriterBase&, const TUnderlying&, size_t)>&& transform)
			: m_begin(begin)
			, m_end(end)
			, m_options(options)
			, m_transform(std::move(transform))
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

				if (index > 0 ||
					!m_options.skipFirstItemBeginSeparator)
				{
					writer << m_options.chunkBeginSeparator;
				}

				m_transform(writer, *item, index);

				index++;
			}

			if (index > 0 &&
				!m_options.skipLastItemEndSeparator)
			{
				writer << m_options.chunkEndSeparator;
			}
		}

	private:
		TIterator m_begin;
		TIterator m_end;
		CommaListOptions m_options;
		std::function<void(WriterBase&, const TUnderlying&, size_t)> m_transform;

	};

};
