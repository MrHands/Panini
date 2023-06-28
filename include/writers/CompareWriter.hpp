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

#include "data/CompareWriterConfig.hpp"
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Writes output to a path only when the written bytes differ from
		what was loaded on disk.

		The CompareWriter stores the contents of the target path first. When
		the new output differs from what was seen before, the output will be
		committed to the path.

		The writer can be configured with \ref CompareWriterConfig.
	*/

	class CompareWriter
		: public Writer
	{

	public:
		/*!
			Construct and configure the writer.

			\param config    Configuration instance.
		*/
		inline CompareWriter(const CompareWriterConfig& config = {})
			: CompareWriter(config.filePath, config)
		{
			m_compareConfig = config;

			Initialize();
		}

		/*!
			\deprecated This constructor will be removed in the next major
			release. Prefer using the constructor that takes
			\ref CompareWriterConfig.

			Construct and configure the writer.

			\param filePath  File that will be compared against the output.
			\param config    Configuration instance.
		*/
		inline CompareWriter(const std::filesystem::path& filePath, const Config& config = Config())
			: Writer(config)
		{
			m_compareConfig.filePath = filePath;

			Initialize();
		}

		/*!
			Will call Commit() automatically when the writer is destroyed.
		*/
		inline virtual ~CompareWriter() override
		{
			Commit();
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
		inline virtual void Initialize()
		{
			// read the previous output, if available

			std::ifstream stream(m_compareConfig.filePath.string(), std::ios::binary);
			m_pathExists = stream.is_open();
			if (m_pathExists)
			{
				stream.seekg(0, std::ios::end);
				m_writtenPrevious.resize(static_cast<size_t>(stream.tellg()));
				stream.seekg(0, std::ios::beg);
				stream.read(&m_writtenPrevious[0], m_writtenPrevious.size());
				stream.close();

				m_writtenCurrent.reserve(m_writtenPrevious.size());
			}
		}

		inline virtual void Write(const std::string& chunk) override
		{
			m_writtenCurrent += chunk;
		}

		inline virtual bool OnCommit(bool force = false) override
		{
			(void)force;

			std::ofstream stream(m_compareConfig.filePath.string(), std::ios::binary);
			if (!stream.is_open())
			{
				return false;
			}

			stream.write(m_writtenCurrent.c_str(), m_writtenCurrent.length());
			stream.close();

			m_writtenPrevious = m_writtenCurrent;

			return true;
		}

	protected:
		CompareWriterConfig m_compareConfig;
		bool m_pathExists = false;
		std::string m_writtenPrevious;
		std::string m_writtenCurrent;

	};

};
