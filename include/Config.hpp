#pragma once

// C/STL

#include <functional>
#include <stdint.h>
#include <string>

namespace panini
{

	enum class BraceBreakingStyle
	{
		Attach, // Open brace on the same line
		Allman, // New line before brace open
		Whitesmiths // New line and indent before brace open and brace close
	};

	struct Config
	{
		BraceBreakingStyle braces = BraceBreakingStyle::Allman;
		std::string chunkNewLine = "\n";
		std::string chunkIndent = "\t";
	};

};
