#pragma once

#include <fstream>

#include "writers/WriterBase.hpp"

namespace panini
{

	class CompareWriter
		: public WriterBase
	{

	public:
		CompareWriter(const std::string& fileName, const Config& config = Config())
			: WriterBase(config)
			, m_fileName(fileName)
		{
			std::ifstream stream(m_fileName.c_str(), std::ios::binary);
			if (stream.is_open())
			{
				stream >> m_writtenPrevious;
				stream.close();
			}
		}

		~CompareWriter()
		{
			Commit();
		}

		virtual void Write(const std::string& chunk) final
		{
			m_writtenCurrent += chunk;
		}

		bool Commit()
		{
			if (m_writtenPrevious == m_writtenCurrent)
			{
				return true;
			}

			std::ofstream stream(m_fileName, std::ios::binary);
			if (!stream.is_open())
			{
				return false;
			}

			stream << m_writtenCurrent;
			stream.close();

			m_writtenPrevious = m_writtenCurrent;

			return true;
		}

	private:
		std::string m_fileName;
		std::string m_writtenPrevious;
		std::string m_writtenCurrent;

	};

};
