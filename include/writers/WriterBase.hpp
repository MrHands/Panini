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

#include "commands/CommandBase.hpp"
#include "commands/IndentPop.hpp"
#include "commands/IndentPush.hpp"
#include "commands/NextLine.hpp"
#include "Config.hpp"

namespace panini
{
	/*!
		\brief Base class for writers.

		Writers take chunks and commands as input and process them to a target. They
		are configured with a \ref Config instance that is passed to their
		constructor.

		Writers commit their output to a target automatically when they are
		destroyed.
	*/
	class WriterBase
	{

	public:
		/*!
			Constructs a writer with an optional configuration instance.
		*/
		inline explicit WriterBase(const Config& config = Config{})
			: m_config(config)
		{
			// reserve cached strings

			const size_t indentReserveSize = 8 * m_config.chunkIndent.size();
			m_lineIndentCached.reserve(indentReserveSize);
			m_commentIndentCached.reserve(indentReserveSize);

			// inherit is not allowed on the config

			if (m_config.braceBreakingStyle == BraceBreakingStyle::Inherit)
			{
				Config defaultConfig;
				m_config.braceBreakingStyle = defaultConfig.braceBreakingStyle;
			}

			if (m_config.includeStyle == IncludeStyle::Inherit)
			{
				Config defaultConfig;
				m_config.includeStyle = defaultConfig.includeStyle;
			}
		}

		/*!
			The output is committed to the path automatically when the instance
			is destroyed.
		*/
		virtual ~WriterBase()
		{
			Commit();
		}

		/*!
			Get the default brace breaking style.

			\return Brace breaking style.
		*/
		inline BraceBreakingStyle GetBraceBreakingStyle() const
		{
			return m_config.braceBreakingStyle;
		}

		/*!
			Get the default include style.

			\return Include style.
		*/
		inline IncludeStyle GetIncludeStyle() const
		{
			return m_config.includeStyle;
		}

		/*!
			Check whether the writer is on a new line and waiting for new
			chunks.

			\return True if the writer is on a new line
		*/
		inline bool IsOnNewLine() const
		{
			return m_state == State::NewLine;
		}

		/*!
			Write an `std::string` chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		inline WriterBase& operator << (const std::string& chunk)
		{
			if (m_state == State::NewLine)
			{
				if (!m_lineIndentCached.empty())
				{
					Write(m_lineIndentCached);
				}

				m_state = State::Chunk;

				if (m_isInCommentBlock)
				{
					Write(" * ");

					if (!m_commentIndentCached.empty())
					{
						Write(m_commentIndentCached);
					}
				}
			}

			Write(chunk);

			m_lineChunkCountWritten += chunk.size();

			return *this;
		}

		/*!
			Write a C-style string chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		inline WriterBase& operator << (const char* chunkString)
		{
			return *this << std::string(chunkString);
		}

		/*!
			Write a new line chunk to the output.

			New line chunks can be configured with the @ref Config.

			\return Reference to itself to allow for chaining.
		*/
		inline WriterBase& operator << (const NextLine& command)
		{
			if (m_lineChunkCountWritten == 0 &&
				m_isInCommentBlock)
			{
				// edge-case for empty lines within a comment block

				Write(" *");
			}

			Write(m_config.chunkNewLine);

			m_state = State::NewLine;

			m_lineChunkCountWritten = 0;

			return *this;
		}

		/*!
			Increment the level of indentation.

			Indentation is applied only when the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		inline WriterBase& operator << (const IndentPush& command)
		{
			if (!m_isInCommentBlock)
			{
				CacheIndentation(m_lineIndentCached, ++m_lineIndentCount);
			}
			else
			{
				CacheIndentation(m_commentIndentCached, ++m_commentIndentCount);
			}

			return *this;
		}

		/*!
			Decrement the level of indentation.

			Indentation is applied when the writer is on a new line. The level
			of indentation cannot go negative.

			\return Reference to itself to allow for chaining.
		*/
		inline WriterBase& operator << (const IndentPop& command)
		{
			if (!m_isInCommentBlock)
			{
				if (m_lineIndentCount > 0)
				{
					CacheIndentation(m_lineIndentCached, --m_lineIndentCount);
				}
			}
			else
			{
				if (m_commentIndentCount > 0)
				{
					CacheIndentation(m_commentIndentCached, --m_commentIndentCount);
				}
			}

			return *this;
		}

		/*!
			Visit a command.

			\warning Commands are moved instead of copied!

			Commands are used to output chunks and modify the writer's state.

			\return Reference to itself to allow for chaining
		*/
		inline WriterBase& operator << (CommandBase&& command)
		{
			command.Visit(*this);

			return *this;
		}

		/*!
			Set the writer to be inside a comment block, which will add " * "
			after the indentation of a new line.
		*/
		inline void SetIsInCommentBlock(bool value)
		{
			m_isInCommentBlock = value;

			m_commentIndentCount = 0;
			m_commentIndentCached.clear();
		}

		/*!
			Check if the output was changed compared to what the implementation has
			seen before.
		*/
		inline virtual bool IsChanged() const
		{
			return true;
		}

		/*!
			Commits the generated output to the target of a writer if the
			output was changed.

			@param force Force writing the file even if the output was not
			changed.

			@return Returns true if the commit was successful.
		*/
		inline bool Commit(bool force = false)
		{
			return (IsChanged() || force) && OnCommit(force);
		}

	protected:
		/*!
			Interface for writing chunks to the output. Must be implemented by
			derived classes.
		*/
		virtual void Write(const std::string& chunk) = 0;

		/*!
			Called when the writer is committed to its target. Must be
			implemented by derived classes.
		*/
		virtual bool OnCommit(bool force = false) = 0;

	private:
		/*!
			Cache the indentation level to the target string.
		*/
		inline void CacheIndentation(std::string& target, int32_t count)
		{
			target.clear();

			for (int32_t i = 0; i < count; ++i)
			{
				target += m_config.chunkIndent;
			}
		}

	private:
		Config m_config;

		size_t m_lineChunkCountWritten = 0;

		int32_t m_lineIndentCount = 0;
		std::string m_lineIndentCached;

		enum class State
		{
			NewLine,
			Chunk
		};
		State m_state = State::NewLine;

		bool m_isInCommentBlock = false;
		int32_t m_commentIndentCount = 0;
		std::string m_commentIndentCached;

	};

};
