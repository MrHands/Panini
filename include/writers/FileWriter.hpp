#pragma once

#include <fstream>

#include "writers/WriterBase.hpp"

namespace panini
{

	class FileWriter
		: public WriterBase
	{

	public:
		FileWriter(const std::string& fileName, const Config& config = Config())
			: WriterBase(config)
		{
			m_stream.open(fileName.c_str());
		}

		~FileWriter()
		{
			Close();
		}

		virtual void Write(const std::string& chunk) final
		{
			if (!m_stream.is_open())
			{
				return;
			}

			m_stream << chunk;
		}

		void Close()
		{
			m_stream.close();
		}

	private:
		std::ofstream m_stream;

	};

};
