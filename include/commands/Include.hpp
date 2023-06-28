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

#include "commands/Command.hpp"
#include "data/IncludeEntry.hpp"
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Command for outputting an include statement for C++.

		Include statements can be output by hand with relative ease, but this
		command will help you standardize how they are written.

		Includes use double quotes by default, and can use single quotes or
		angular brackets as well.

		\note Include statements with single quotes are not valid C++.

		Example:

		\code{.cpp}
			writer << Include("stdio.h", IncludeStyle::AngularBrackets);
			writer << Include("game/systems/Audio.h") << NextLine();
			writer << Include("game/systems/Particles.h") << NextLine();
			writer << Include("game/Physics.h") << NextLine();
		\endcode

		Output:

		\code{.cpp}
			#include <stdio.h>
			#include "game/systems/Audio.h"
			#include "game/systems/Particles.h"
			#include "game/Physics.h"
		\endcode
	*/
	class Include
		: public Command
	{

	public:
		/*!
			Construct an Include command from an IncludeEntry that is copied
			to the instance.
		*/
		inline Include(const IncludeEntry& entry)
			: m_entry(entry)
		{
		}

		/*!
			Construct an Include command from an IncludeEntry that is moved
			into the instance.
		*/
		inline Include(IncludeEntry&& entry) noexcept
			: m_entry(std::exchange(entry, {}))
		{
		}

		/*!
			Create an Include command with a path and an IncludeStyle.

			Setting the `style` parameter to IncludeStyle::Inherit copies the
			include style from the writer, otherwise it will be overridden for
			this command only.
		*/
		inline Include(const std::filesystem::path& path, IncludeStyle style = IncludeStyle::Inherit)
			: m_entry(path, style)
		{
		}

		/*!
			Create an Include command with a path and an IncludeStyle.

			Setting the `style` parameter to IncludeStyle::Inherit copies the
			include style from the writer, otherwise it will be overridden for
			this command only.
		*/
		inline Include(std::filesystem::path&& path, IncludeStyle style = IncludeStyle::Inherit) noexcept
			: m_entry(path, style)
		{
		}

		inline virtual void Visit(Writer& writer) final
		{
			IncludeStyle includeStyle =
				m_entry.style == IncludeStyle::Inherit
					? writer.GetIncludeStyle()
					: m_entry.style;

			writer << "#include ";

			std::string path = m_entry.path.string();

			switch (includeStyle)
			{

			case IncludeStyle::DoubleQuotes:
				writer << "\"" << path << "\"";
				break;

			case IncludeStyle::SingleQuotes:
				writer << "'" << path << "'";
				break;

			case IncludeStyle::AngularBrackets:
				writer << "<" << path << ">";
				break;
			
			default:
				break;

			}
		}

	private:
		IncludeEntry m_entry;

	};

};
