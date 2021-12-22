#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class CommentLine
		: public CommandBase
	{

	public:
		CommentLine() = default;
		CommentLine(const std::string& comment)
			: m_comment(comment)
		{
		}
		CommentLine(std::string&& comment)
			: m_comment(comment)
		{
		}

		virtual void Visit(WriterBase& writer) final
		{
			if (m_comment.empty())
			{
				return;
			}

			writer << "// " << m_comment;
		}

	private:
		std::string m_comment;

	};

};
