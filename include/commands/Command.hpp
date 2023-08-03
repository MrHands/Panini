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

#include "data/WriterConfig.hpp"

namespace panini
{

	class Writer;

	/*!
		\brief Base class for commands.

		Commands are used to output chunks to the writer. They should be
		designed to be chained together. Because commands cannot undo output
		that was already written, you should be careful not to output more than
		you strictly need. A good rule of thumb is that a command should never
		end on a \ref NextLine command.

		\sa Writer
	*/

	class Command
	{

	public:
		virtual ~Command() = default;

		/*!
			Accepts a \ref Writer to write chunks to the output.
		*/
		virtual void Visit(Writer& writer) = 0;

	};

	//! \deprecated Prefer using \ref Command instead
	using CommandBase = Command;

};
