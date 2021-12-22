#pragma once

#include "commands/Braces.hpp"
#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class Scope
		: public CommandBase
	{

	public:
		Scope(std::string&& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle braceStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(callback)
			, m_braceStyle(braceStyle)
		{
		}
		Scope(const std::string& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle braceStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(callback)
			, m_braceStyle(braceStyle)
		{
		}

		virtual void Visit(WriterBase& writer) final
		{
			BraceBreakingStyle braceStyle =
				m_braceStyle == BraceBreakingStyle::Inherit
					? writer.GetBraceStyle()
					: m_braceStyle;

			if (!m_name.empty())
			{
				writer << m_name;

				if (braceStyle == BraceBreakingStyle::Attach)
				{
					writer << " ";
				}
			}

			writer << Braces(std::move(m_callback), braceStyle);
		}

	private:
		std::string m_name;
		std::function<void(WriterBase&)> m_callback;
		BraceBreakingStyle m_braceStyle;

	};

};
