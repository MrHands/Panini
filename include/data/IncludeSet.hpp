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

#pragma once

#include "data/IncludeEntry.hpp"

#include <filesystem>

namespace panini
{

	/*!
		\brief Collection of unique file system paths.

		\ingroup Data

		Paths can be sorted by priority, which is based on their IncludeStyle
		and how many forward slashes (/) it contains. This causes paths that
		end in a filename to be sorted _after_ paths with folders, even when
		the folder name is the same as the filename.

		\note Duplicate paths are not allowed in the collection, unless they
		differ in IncludeStyle.
	*/

	class IncludeSet
	{

	public:
		inline IncludeSet() = default;

		/*!
			Construct an IncludeSet from a list of IncludeEntry.
		*/
		inline IncludeSet(std::initializer_list<IncludeEntry> entries)
		{
			for (const IncludeEntry& entry : entries)
			{
				Add(entry.path, entry.style);
			}
		}

		/*!
			Construct an IncludeSet from a list of paths and an IncludeStyle.
		*/
		inline IncludeSet(
			std::initializer_list<std::filesystem::path> paths,
			IncludeStyle style)
		{
			for (const std::filesystem::path& path : paths)
			{
				Add(path, style);
			}
		}

		/*!
			Get entries as an std::vector.
		*/
		inline const std::vector<IncludeEntry>& GetEntries() const
		{
			return m_entries;
		}

		/*!
			STL-compatible method for getting an iterator to the start of the
			list of entries.
		*/
		inline std::vector<IncludeEntry>::const_iterator begin() const
		{
			return m_entries.begin();
		}

		/*!
			STL-compatible method for getting an iterator to the start of the
			list of entries.
		*/
		inline std::vector<IncludeEntry>::iterator begin()
		{
			return m_entries.begin();
		}

		/*!
			STL-compatible method for getting an iterator to the end of the
			list of entries.
		*/
		inline std::vector<IncludeEntry>::const_iterator end() const
		{
			return m_entries.end();
		}

		/*!
			STL-compatible method for getting an iterator to the end of the
			list of entries.
		*/
		inline std::vector<IncludeEntry>::iterator end()
		{
			return m_entries.end();
		}

		/*!
			Add a path to the set with an include style.

			\note Duplicate paths will note be added, unless they differ in
			IncludeStyle.
		*/
		inline void Add(
			const std::filesystem::path& path,
			IncludeStyle style = IncludeStyle::Inherit)
		{
			// check if the path is not already known

			auto found = std::find_if(
				m_entries.begin(),
				m_entries.end(),
				[&path, &style](const IncludeEntry& it) {
					return it.path == path && it.style == style;
				}
			);
			if (found != m_entries.end())
			{
				return;
			}

			// add new entry to list

			m_entries.emplace_back(IncludeEntry{ path, style });
		}

		/*!
			Sort the paths in the collection based on an include style.

			Entries are first sorted by include style:

			1. IncludeStyle::AngularBrackets
			2. IncludeStyle::DoubleQuotes
			3. IncludeStyle::SingleQuotes

			And then by their paths.

			The resolved style parameter is used to prioritize entries that
			have an include style of IncludeStyle::Inherit.

			\note You cannot sort with a resolved style set to
			IncludeStyle::Inherit.
		*/
		inline void Sort(IncludeStyle resolvedStyle)
		{
			if (resolvedStyle == IncludeStyle::Inherit)
			{
				return;
			}

			// resolve priority for all entries

			for (IncludeEntry& entry : m_entries)
			{
				IncludeStyle style =
					(entry.style == IncludeStyle::Inherit)
						? resolvedStyle
						: entry.style;

				// base priority on include style

				entry.priority = 1000;

				switch (style)
				{

				case IncludeStyle::AngularBrackets:
					entry.priority = 0;
					break;

				case IncludeStyle::DoubleQuotes:
					entry.priority = 100;
					break;

				case IncludeStyle::SingleQuotes:
					entry.priority = 200;
					break;

				default:
					break;

				}

				// paths with folders should come before files

				const std::string path = entry.path.string();

				size_t offset = 0;
				size_t next = 0;
				while ((next = path.find_first_of('/', offset)) != std::string::npos)
				{
					entry.priority--;
					offset = next + 1;
				}
			}

			// sort by priority and path

			std::sort(
				m_entries.begin(),
				m_entries.end(),
				[](IncludeEntry& left, IncludeEntry& right) {
					if (left.priority != right.priority)
					{
						return left.priority < right.priority;
					}
					else
					{
						return left.path < right.path;
					}
				}
			);
		}

	private:
		std::vector<IncludeEntry> m_entries;

	};

};
