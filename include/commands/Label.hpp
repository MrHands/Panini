#pragma once

#include "commands/CommandBase.hpp"

namespace panini
{

	class Label
		: public CommandBase
	{

	public:
		Label(std::string&& name)
			: m_name(name)
		{
		}

		Label(const std::string& name)
			: m_name(name)
		{
		}

		virtual void Visit(WriterBase& writer) final
		{
			writer << IndentPop() << m_name << ":" << IndentPush();
		}

	private:
		std::string m_name;

	};

};
