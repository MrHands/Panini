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
			writer << "{" << IndentPush() << NextLine();
			m_callback(writer);
			writer << IndentPop() << "}";
		}

	private:
		std::function<void(WriterBase&)> m_callback;

	};

};
