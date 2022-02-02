/*
	MIT No Attribution

	Copyright 2021-2022 Mr. Hands

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to
	deal in the Software without restriction, including without limitation the
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

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
