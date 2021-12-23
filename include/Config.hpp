#pragma once

// C/STL

#include <functional>
#include <stdint.h>
#include <string>

/*!
	\file
	\defgroup Enums Global enumeration values.
*/

namespace panini
{
	/*!
		\brief Brace breaking style to use when writing to output.
		\ingroup Enums
	*/
	enum class BraceBreakingStyle
	{
		Inherit,      //!< Inherit setting from the config, not valid on @ref WriterBase
		Attach,       //!< Open brace on the same line
		Allman,       //!< New line before brace open
		Whitesmiths   //!< New line and indent before brace open and brace close
	};

	/*!
		\brief Include style to use when writing to output.
		\ingroup Enums
	*/
	enum class IncludeStyle
	{
		DoubleQuotes,    //!< Output double quotation marks ""
		SingleQuotes,    //!< Output single quotation marks '' (not valid in C++)
		AngularBrackets  //!< Output angular brackets <>
	};

	/*!
		\brief Global configuration applied to writers.

		Configuration is applied to all commands when they are processed by
		@ref WriterBase. Some commands can override these configuration
		settings.
	*/
	struct Config
	{
		BraceBreakingStyle braces = BraceBreakingStyle::Allman;
		std::string chunkNewLine = "\n";
		std::string chunkIndent = "\t";
	};

};
