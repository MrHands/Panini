#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Outputs an include statement for C++.

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
		: public CommandBase
	{

	public:
		/*!
			Create an Include command with a `fileName` that is copied to the
			instance.

			Setting the `style` parameter to IncludeStyle::Inherit copies the
			include style from the writer, otherwise it will be overridden for
			this command only.
		*/
		inline Include(const std::string& fileName, IncludeStyle style = IncludeStyle::Inherit)
			: m_fileName(fileName)
			, m_includeStyle(style)
		{
		}

		/*!
			Create an Include command with a `fileName` that is moved into the
			instance.

			Setting the `style` parameter to IncludeStyle::Inherit copies the
			include style from the writer, otherwise it will be overridden for
			this command only.
		*/
		inline Include(std::string&& fileName, IncludeStyle style = IncludeStyle::Inherit)
			: m_fileName(fileName)
			, m_includeStyle(style)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			IncludeStyle includeStyle =
				m_includeStyle == IncludeStyle::Inherit
					? writer.GetIncludeStyle()
					: m_includeStyle;

			writer << "#include ";

			switch (includeStyle)
			{

			case IncludeStyle::DoubleQuotes:
				writer << "\"" << m_fileName << "\"";
				break;

			case IncludeStyle::SingleQuotes:
				writer << "'" << m_fileName << "'";
				break;

			case IncludeStyle::AngularBrackets:
				writer << "<" << m_fileName << ">";
				break;
			
			default:
				break;

			}
		}

	private:
		std::string m_fileName;
		IncludeStyle m_includeStyle;

	};

};
