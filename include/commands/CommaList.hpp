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

namespace panini
{

	template <typename TItem>
	class CommaList
		: public CommandBase
	{

	public:
		inline CommaList(const std::vector<TItem>& items, const std::string& separator = ", ")
			: m_items(items)
			, m_separator(separator)
		{
		}

		inline CommaList(const std::vector<TItem>& items, std::function<std::string<const TItem&>> transform, const std::string& separator = ", ")
			: m_items(items)
			, m_transform(transform)
			, m_separator(separator)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			if (m_transform)
			{
				size_t itemIndex = 0;
				for (const TItem& item : m_items)
				{
					if (itemIndex++ > 0)
					{
						writer << m_separator;
					}

					writer << m_transform(item);
				}
			}
			else
			{
				size_t itemIndex = 0;
				for (const TItem& item : m_items)
				{
					if (itemIndex++ > 0)
					{
						writer << m_separator;
					}

					writer << item;
				}
			}
		}

	private:
		std::vector<TItem> m_items;
		std::string m_separator;
		std::function<std::string<const TItem&>> m_transform;

	};

};
