#pragma once

// C/STL

#include <functional>
#include <stdint.h>
#include <string>

namespace panini
{

	enum class BraceBreakingStyle
	{
		Inherit, // Inherit setting from the config
		Attach, // Open brace on the same line
		Allman, // New line before brace open
		Whitesmiths // New line and indent before brace open and brace close
	};

	enum class IncludeStyle
	{
		DoubleQuotes,
		SingleQuotes,
		AngularBrackets
	};

	struct Config
	{
		BraceBreakingStyle braces = BraceBreakingStyle::Allman;
		std::string chunkNewLine = "\n";
		std::string chunkIndent = "\t";
	};

};
