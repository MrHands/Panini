#pragma once

// C/STL

#include <functional>
#include <stdint.h>
#include <string>

namespace panini
{
	struct Config
	{
		std::string chunkNewLine = "\n";
		std::string chunkIndent = "\t";
	};
}