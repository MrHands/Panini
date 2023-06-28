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
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Command for outputting comment blocks over multiple lines.

		A comment block starts with /&zwj;* and ends with *&zwj;/. Chunks output
		inside the callback are prefixed with " * " to produce multi-line
		comments.

		You can still use other commands inside the comment block, including
		indentation. Note that the command adds a new line automatically.

		Example:

		\code{.cpp}
			writer << CommentBlock([](Writer& writer) {
				writer << "EXAMPLE:" << NextLine();
				writer << NextLine();
				writer << "Writing beautiful multi-line comments is easy" << NextLine();
				writer << "When you use a comment block command!";
			});
		\endcode

		Output:

		\code{.cpp}
			/* EXAMPLE:
			 *
			 * Writing beautiful multi-line comments is easy
			 * When you use a comment block command!
			 *\/
		\endcode
	*/
	class CommentBlock
		: public Command
	{

	public:
		/*!
			Construct a CommentBlock with a callback that is moved into the
			instance.

			The callback is called when the command is visited by a \ref Writer.
			Chunks output inside the callback will be prefixed with the C multi-
			line comment syntax.
		*/
		inline explicit CommentBlock(std::function<void(Writer&)>&& callback) noexcept
			: m_callback(std::exchange(callback, {}))
		{
		}

		inline virtual void Visit(Writer& writer) final
		{
			writer << "/* ";
			writer.SetIsInCommentBlock(true);

			m_callback(writer);

			writer.SetIsInCommentBlock(false);
			writer << NextLine() << " */";
		}

	private:
		std::function<void(Writer&)> m_callback;

	};

};
