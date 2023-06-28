/*
	MIT No Attribution

	Copyright 2021-2023 Mr. Hands

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

#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Writes output to the console.

		The ConsoleWriter will write to `std::cout` by default, which is the
		default output stream for console programs. You can write to a different
		stream by supplying it as the first argument to the constructor.
	*/
	class ConsoleWriter
		: public Writer
	{

	public:
		/*!
			Write to the default output stream for console programs.

			\param config  Configuration instance
		*/
		inline explicit ConsoleWriter(const WriterConfig& config = WriterConfig())
			: Writer(config)
			, m_outputStream(std::cout)
		{
		}

		/*!
			Configure the output stream to use for the writer.

			\param outputStream Stream used for writing
			\param config Configuration instance
		*/
		inline explicit ConsoleWriter(
			std::ostream& outputStream,
			const WriterConfig& config = WriterConfig())
			: Writer(config)
			, m_outputStream(outputStream)
		{
		}

	protected:
		/*!
			Writes the chunk to the console stream.
		*/
		inline void Write(const std::string& chunk) override
		{
			m_outputStream << chunk;
		}

		/*
			Flush the console output on a new line.
		*/
		inline void WriteNewLine() override
		{
			m_outputStream << std::endl;
		}

		/*
			Always commit to the console when the writer is destroyed.
		*/
		inline bool OnCommit(bool force) override
		{
			(void)force;

			return true;
		}

	protected:
		std::ostream& m_outputStream;

	};

};
