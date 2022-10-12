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

namespace panini
{

	/*!
		\brief Options for the CommaList command.
	*/
	struct CommaListOptions
	{
		/*!
			Chunk inserted before each item in the list, including the first
			item.
		*/
		std::string chunkBeginSeparator = "";

		/*!
			Chunk inserted after the first item in the list, excluding the last
			item in the list.
		*/
		std::string chunkEndSeparator = ", ";

		/*!
			Whether to add NextLine commands after each item in the list.
		*/
		bool addNewLines = false;

		bool skipFirstItemBeginSeparator = false;
	};

};
