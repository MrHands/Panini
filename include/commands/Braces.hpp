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
			const bool wasNewLine = writer.IsOnNewLine();

			switch (writer.GetBraceStyle())
			{

			case BraceBreakingStyle::Attach:
				{
					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Allman:
				{
					if (!wasNewLine)
					{
						writer << NextLine();
					}

					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Whitesmiths:
				{
					if (!wasNewLine)
					{
						writer << NextLine() << IndentPush();
					}

					writer << "{" << NextLine();
					m_callback(writer);
					writer << "}";

					if (!wasNewLine)
					{
						writer << IndentPop();
					}

				} break;

			default:
				break;
			}
		}

	private:
		std::function<void(WriterBase&)> m_callback;

	};

};
