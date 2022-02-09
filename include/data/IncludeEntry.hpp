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

#include "Config.hpp"

namespace panini
{

	/*!
		\brief Data entry for IncludeCollection.
	*/
	struct IncludeEntry
	{
		inline IncludeEntry() = default;

		inline IncludeEntry(const std::filesystem::path& _path, IncludeStyle _style = IncludeStyle::AngularBrackets, int32_t _priority = 0)
			: path(_path)
			, style(_style)
			, priority(_priority)
		{
		}

		inline IncludeEntry(std::filesystem::path&& _path, IncludeStyle _style = IncludeStyle::AngularBrackets, int32_t _priority = 0) noexcept
			: path(std::exchange(_path, {}))
			, style(_style)
			, priority(_priority)
		{
		}

		inline IncludeEntry(const std::string& _path, IncludeStyle _style = IncludeStyle::AngularBrackets, int32_t _priority = 0)
			: path(_path)
			, style(_style)
			, priority(_priority)
		{
		}

		inline IncludeEntry(std::string&& _path, IncludeStyle _style = IncludeStyle::AngularBrackets, int32_t _priority = 0) noexcept
			: path(_path)
			, style(_style)
			, priority(_priority)
		{
		}

		inline IncludeEntry(const IncludeEntry& other)
			: path(other.path)
			, style(other.style)
			, priority(other.priority)
		{
		}

		inline IncludeEntry(IncludeEntry&& other) noexcept
			: path(std::exchange(other.path, {}))
			, style(std::exchange(other.style, {}))
			, priority(std::exchange(other.priority, {}))
		{
		}

		inline IncludeEntry& operator = (const IncludeEntry& other)
		{
			return *this = IncludeEntry{ other };
		}

		inline IncludeEntry& operator = (IncludeEntry&& other) noexcept
		{
			std::swap(path, other.path);
			std::swap(style, other.style);
			std::swap(priority, other.priority);

			return *this;
		}

		std::filesystem::path path;
		IncludeStyle style = IncludeStyle::AngularBrackets;
		int32_t priority = 0;
	};

};
