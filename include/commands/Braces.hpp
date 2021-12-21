#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class Braces
		: public CommandBase
	{

	public:
		Braces(std::function<void(WriterBase&)>&& callback)
			: m_callback(callback)
		{
		}

		virtual void Visit(WriterBase& writer) final
		{
			switch (writer.GetBraces())
			{

			case BraceBreakingStyle::Attach:
				{
					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Allman:
				{
					writer << NextLine();
					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Whitesmiths:
				{

				} break;

			default:
				break;
			}

			
		}

	private:
		std::function<void(WriterBase&)> m_callback;

	};

};
