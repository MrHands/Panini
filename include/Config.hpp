#pragma once

// C/STL

#include <functional>
#include <stdint.h>
#include <string>

/*!
	\file Config.hpp
	\defgroup Enums Global enumeration values
*/

namespace panini
{
	/*!
		\enum BraceBreakingStyle
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
		\enum IncludeStyle
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
		\ref WriterBase. Some commands can override these configuration
		settings.
	*/
	struct Config
	{
		/*!
			The default brace breaking style to use for a writer.
		*/
		BraceBreakingStyle braceBreakingStyle = BraceBreakingStyle::Allman;

		/*!
			Chunk to output on a new line.
		*/
		std::string chunkNewLine = "\n";

		/*!
			Chunk to output when indenting.
		*/
		std::string chunkIndent = "\t";
	};

};
