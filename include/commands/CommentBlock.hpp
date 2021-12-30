#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class CommentBlock
		: public CommandBase
	{

	public:
		CommentBlock() = default;
		CommentBlock(std::function<void(WriterBase&)>&& callback)
			: m_callback(callback)
		{
		}

		virtual void Visit(WriterBase& writer) final
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
