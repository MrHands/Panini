#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Output comment blocks over multiple lines.

		A comment block starts with "/*" and ends with \c "*\/". Chunks output
		inside the callback are prefixed with " * " to produce multi-line
		comments.

		You can still use other commands inside the comment block, including
		indentation. Note that the command adds a new line automatically.

		Example:

		\code{.cpp}
			writer << CommentBlock([](WriterBase& writer) {
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
		: public CommandBase
	{

	public:
		/*!
			Construct a CommentBlock with a callback that is moved into the
			instance.

			The callback is called when the command is visited by a
			\ref WriterBase. Chunks output inside the callback will be prefixed
			with the C multi-line comment syntax.
		*/
		inline explicit CommentBlock(std::function<void(WriterBase&)>&& callback)
			: m_callback(callback)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			writer << "/* ";
			writer.SetIsInCommentBlock(true);

			m_callback(writer);

			writer.SetIsInCommentBlock(false);
			writer << NextLine() << " */";
		}

	private:
		std::function<void(WriterBase&)> m_callback;

	};

};
