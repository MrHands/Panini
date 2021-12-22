#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class Include
		: public CommandBase
	{

	public:
		Include(const std::string& fileName, IncludeStyle style = IncludeStyle::DoubleQuotes)
			: m_fileName(fileName)
			, m_style(style)
		{
		}
		Include(std::string&& fileName, IncludeStyle style = IncludeStyle::DoubleQuotes)
			: m_fileName(fileName)
			, m_style(style)
		{
		}

		virtual void Visit(WriterBase& writer) final
		{
			writer << "#include ";

			switch (m_style)
			{

			case IncludeStyle::DoubleQuotes:
				writer << "\"" << m_fileName << "\"";
				break;

			case IncludeStyle::SingleQuotes:
				writer << "'" << m_fileName << "'";
				break;

			case IncludeStyle::AngularBrackets:
				writer << "<" << m_fileName << ">";
				break;
			
			default:
				break;

			}
		}

	private:
		std::string m_fileName;
		IncludeStyle m_style;

	};

};
