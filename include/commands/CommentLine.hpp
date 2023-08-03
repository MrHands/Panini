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

#include "commands/Command.hpp"
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Command for outputting a single-line comment.

		\ingroup Commands

		Comment lines start with "// " in the output. Empty lines will be
		output as well.

		Example:

		\code{.cpp}
			writer << CommentLine("I can't believe this was generated!");
		\endcode

		Output:

		\code{.cpp}
			// I can't believe this was generated!
		\endcode
	*/

	class CommentLine
		: public Command
	{

	public:
		/*!
			Construct a CommentLine with a comment that is copied to the
			instance.
		*/
		inline explicit CommentLine(const std::string& comment)
			: m_comment(comment)
		{
		}

		/*!
			Construct a CommentLine with a comment that is moved into the
			instance.
		*/
		inline explicit CommentLine(std::string&& comment) noexcept
			: m_comment(std::exchange(comment, {}))
		{
		}

		inline void Visit(Writer& writer) override
		{
			writer << "//";

			if (!m_comment.empty())
			{
				writer << " " << m_comment;
			}
		}

	private:
		std::string m_comment;

	};

};
