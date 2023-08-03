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

#include <string>

#include "data/BraceBreakingStyle.hpp"

namespace panini
{

	/*!
		\brief Options for the \ref Braces command.

		\ingroup CommandOptions
	*/

	struct BracesOptions
	{
		/*!
			Brace breaking style to use for the command.

			When this parameter is set to \ref BraceBreakingStyle::Inherit,
			the brace breaking style is copied from the active writer.
		*/
		BraceBreakingStyle breakingStyle = BraceBreakingStyle::Inherit;

		/*!
			Chunk inserted to open the scope.
		*/
		std::string chunkBraceOpen = "{";

		/*!
			Chunk inserted to close the scope.
		*/
		std::string chunkBraceClose = "}";
	};

};
