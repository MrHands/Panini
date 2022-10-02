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

#include "data/FileWriterConfig.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Writes output to a target file using a file stream.
		
		The file stream is closed automatically when the writer is destroyed.

		The \ref Config instance is used to configure the output.
	*/
	class FileWriter
		: public WriterBase
	{

	public:
		/*!
			Construct and configure the writer.

			\param config    Configuration instance.
		*/
		inline FileWriter(const FileWriterConfig& config = {})
			: FileWriter(config.targetPath, config)
		{
			m_fileConfig = config;
		}

		/*!
			Construct and configure the writer.

			\deprecated This constructor will likely be removed in a future
			version. Prefer initializing the writer with a
			\ref FileWriterConfig instance instead.

			\param path    Path to the target file.
			\param config  Configuration instance.
		*/
		inline FileWriter(const std::filesystem::path& path, const Config& config = Config())
			: WriterBase(config)
		{
			m_target.open(path.string(), std::ios::out | std::ios::binary);
		}

		/*!
			Will call Commit() automatically when the writer is destroyed.
		*/
		inline virtual ~FileWriter() override
		{
			Commit();
		}

		/*!
			Always close the stream when \ref Commit is called.
		*/
		inline virtual bool IsChanged() const override
		{
			return m_target.is_open();
		}

	protected:
		/*!
			Writes the chunk to the file stream.
		*/
		inline virtual void Write(const std::string& chunk) override
		{
			if (!m_target.is_open())
			{
				return;
			}

			m_written += chunk;
		}

		/*!
			Close the file stream when the writer is committed.
		*/
		inline virtual bool OnCommit(bool force) override
		{
			(void)force;

			m_target.write(m_written.c_str(), m_written.length());
			m_target.close();

			return true;
		}

	protected:
		FileWriterConfig m_fileConfig;
		std::ofstream m_target;
		std::string m_written;

	};

};
