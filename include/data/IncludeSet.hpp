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

#include "data/IncludeEntry.hpp"

namespace panini
{

	/*!
		\brief Collection of IncludeEntry.
	*/
	class IncludeSet
	{

	public:
		inline void AddInclude(const std::filesystem::path& path, IncludeStyle style = IncludeStyle::Inherit)
		{
			// check if the path is not already known

			auto found = std::find_if(m_entries.begin(), m_entries.end(), [&path, &style](const IncludeEntry& it) {
				return it.path == path && it.style == style;
			});
			if (found != m_entries.end())
			{
				return;
			}

			// build new entry

			IncludeEntry entry;
			entry.path = path;
			entry.style = style;

			// determine priority based on include style

			entry.priority = 0;

			switch (style)
			{

			case IncludeStyle::AngularBrackets:
				entry.priority = 300;
				break;

			case IncludeStyle::DoubleQuotes:
				entry.priority = 200;
				break;

			case IncludeStyle::SingleQuotes:
				entry.priority = 100;
				break;

			default:
				break;

			}

			// paths with folders should come before files

			std::string regularPath = path.string();

			size_t offset = 0;
			size_t nextSlash = 0;
			while ((nextSlash = regularPath.find_first_of('/', offset)) != std::string::npos)
			{
				entry.priority--;
				offset = nextSlash + 1;
			}

			// add to list

			m_entries.emplace_back(entry);
		}

		inline void SortEntries()
		{
			std::sort(m_entries.begin(), m_entries.end(), [](IncludeEntry& left, IncludeEntry& right) {
				if (left.priority != right.priority)
				{
					return left.priority < right.priority;
				}
				else
				{
					return left.path < right.path;
				}
			});
		}

		inline const std::vector<IncludeEntry>& GetEntries() const
		{
			return m_entries;
		}

	private:
		std::vector<IncludeEntry> m_entries;

	};

};
