#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	class Include
		: public CommandBase
	{

	public:
		inline Include(const std::string& fileName, IncludeStyle style = IncludeStyle::DoubleQuotes)
			: m_fileName(fileName)
			, m_style(style)
		{
		}

		inline Include(std::string&& fileName, IncludeStyle style = IncludeStyle::DoubleQuotes)
			: m_fileName(fileName)
			, m_style(style)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
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
