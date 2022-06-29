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

#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Writes the output to a path only when it was changed.

		The CompareWriter stores the contents of the target path first. When
		the new output differs from what was seen before, the output will be
		committed to the path.

		The \ref Config instance is used to configure the output.
	*/

	class CompareWriter
		: public WriterBase
	{

	public:
		/*!
			Construct a CompareWriter with a file path and a configuration.
		*/
		inline CompareWriter(const std::filesystem::path& filePath, const Config& config = Config())
			: WriterBase(config)
			, m_filePath(filePath)
		{
			std::ifstream stream(m_filePath.string(), std::ios::in | std::ios::binary);
			m_pathExists = stream.is_open();
			if (m_pathExists)
			{
				stream.seekg(0, std::ios::end);
				m_writtenPrevious.resize(stream.tellg());
				stream.seekg(0, std::ios::beg);
				stream.read(&m_writtenPrevious[0], m_writtenPrevious.size());
				stream.close();
			}

			m_writtenCurrent.reserve(m_writtenPrevious.size());
		}

		/*!
			Check if the output was changed compared to what was read from disk
			when the CompareWriter was created.
		*/
		inline virtual bool IsChanged() const override
		{
			return m_writtenPrevious != m_writtenCurrent;
		}

	protected:
		inline virtual void Write(const std::string& chunk) override
		{
			m_writtenCurrent += chunk;
		}

		inline virtual bool OnCommit(bool force = false) override
		{
			if (!IsChanged() &&
				!force)
			{
				return false;
			}

			std::ofstream stream(m_filePath.string(), std::ios::binary);
			if (!stream.is_open())
			{
				return false;
			}

			stream << m_writtenCurrent;
			stream.close();

			m_writtenPrevious = m_writtenCurrent;

			return true;
		}

	protected:
		std::filesystem::path m_filePath;
		bool m_pathExists = false;
		std::string m_writtenPrevious;

		//! Newly generated contents of the file.
		std::string m_writtenCurrent;

	};

};
