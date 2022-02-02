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
