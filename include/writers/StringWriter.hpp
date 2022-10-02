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
		\brief Writes output to a string.

		The \ref Config instance is used to configure the output.
	*/
	class StringWriter
		: public WriterBase
	{

	public:
		/*!
			Construct and configure the writer.

			\param target  String that output will be written to.
			\param config  Configuration instance.
		*/
		inline StringWriter(std::string& target, const Config& config = Config())
			: WriterBase(config)
			, m_target(target)
		{
		}

	protected:
		/*
			Writes the chunk to the target string.
		*/
		inline virtual void Write(const std::string& chunk) override
		{
			m_target += chunk;
		}

		/*!
			Called when the writer is committed.
		*/
		inline virtual bool OnCommit(bool force) override
		{
			(void)force;

			return true;
		}

	protected:
		//! Target string that will be written to.
		std::string& m_target;

	};

};
