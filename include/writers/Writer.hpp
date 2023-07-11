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

#include "commands/Command.hpp"
#include "commands/IndentPop.hpp"
#include "commands/IndentPush.hpp"
#include "commands/NextLine.hpp"
#include "data/WriterConfig.hpp"

namespace panini
{

	/*!
		\brief Pure virtual interface for writers.

		Writers take chunks and commands as input and process them to a target.
		They are configured with a \ref Config instance that is passed to their
		constructor.

		Writers commit their output to a target automatically when they are
		destroyed.
	*/
	class Writer
	{

	public:
		virtual ~Writer() = default;

		/*!
			Get a reference to the active Config for this writer.

			\return Config object.
		*/
		virtual const WriterConfig& GetConfig() const = 0;

		/*!
			Get the default brace breaking style.

			\return Brace breaking style.
		*/
		virtual BraceBreakingStyle GetBraceBreakingStyle() const = 0;

		/*!
			Get the default include style.

			\return Include style.
		*/
		virtual IncludeStyle GetIncludeStyle() const = 0;

		/*!
			Check whether the writer is on a new line and waiting for new
			chunks.

			\return True if the writer is on a new line
		*/
		virtual bool IsOnNewLine() const = 0;

		/*!
			Write an `std::string` chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		virtual Writer& operator << (const std::string& chunk) = 0;

		/*!
			Write a C-style string chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		virtual Writer& operator << (const char* chunkString) = 0;

		/*!
			Write a new line chunk to the output.

			New line chunks can be configured with the \ref Config.

			\return Reference to itself to allow for chaining.
		*/
		virtual Writer& operator << (const NextLine& command) = 0;

		/*!
			Increment the level of indentation.

			Indentation is applied only when the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		virtual Writer& operator << (const IndentPush& command) = 0;

		/*!
			Decrement the level of indentation.

			Indentation is applied when the writer is on a new line. The level
			of indentation cannot go negative.

			\return Reference to itself to allow for chaining.
		*/
		virtual Writer& operator << (const IndentPop& command) = 0;

		/*!
			Visit a command.

			\warning Commands are moved instead of copied!

			Commands are used to output chunks and modify the writer's state.

			\return Reference to itself to allow for chaining
		*/
		virtual Writer& operator << (Command&& command) = 0;

		/*!
			Set the writer to be inside a comment block, which will add " * "
			after the indentation of a new line.
		*/
		virtual void SetIsInCommentBlock(bool value) = 0;

		/*!
			Check if the output was changed compared to what the implementation
			has seen before.
		*/
		virtual bool IsChanged() const = 0;

		/*!
			Commits the generated output to the target of a writer if the
			output was changed.

			\param force Force writing the file even if the output was not
			changed.

			\return Returns true if the commit was successful.
		*/
		virtual bool Commit(bool force) = 0;

	protected:
		/*!
			Writes chunks to the output.
		*/
		virtual void Write(const std::string& chunk) = 0;

		/*!
			Writes a new line chunk to the output.
		*/
		virtual void WriteNewLine() = 0;

		/*!
			Checks if the writer should commit its output to the target.
		*/
		virtual bool OnCommit(bool force = false) = 0;

	};

	/*!
		\brief Base class implementation for writers.

		When making your own writer, you should inherit from this class and
		supply a config struct that inherits from /ref WriterConfig.
	*/
	template <typename TConfig>
	class ConfiguredWriter
		: public Writer
	{

		static_assert(std::is_base_of<WriterConfig, TConfig>::value, "TConfig must inherit from WriterConfig");

	public:
		/*!
			Constructs a writer with an optional configuration instance.
		*/
		inline explicit ConfiguredWriter(const TConfig& config = TConfig{})
			: m_config(config)
		{
			// reserve cached strings

			const size_t indentReserveSize = 8 * m_config.chunkIndent.size();
			m_lineIndentCached.reserve(indentReserveSize);
			m_commentIndentCached.reserve(indentReserveSize);

			// inherit is not allowed on the config

			if (m_config.braceBreakingStyle == BraceBreakingStyle::Inherit)
			{
				TConfig defaultConfig;
				m_config.braceBreakingStyle = defaultConfig.braceBreakingStyle;
			}

			if (m_config.includeStyle == IncludeStyle::Inherit)
			{
				WriterConfig defaultConfig;
				m_config.includeStyle = defaultConfig.includeStyle;
			}
		}

		virtual ~ConfiguredWriter() = default;

		/*!
			Get a reference to the active Config for this writer.

			\return Config object.
		*/
		inline const WriterConfig& GetConfig() const override
		{
			return static_cast<const WriterConfig&>(m_config);
		}

		/*!
			Get the default brace breaking style.

			\return Brace breaking style.
		*/
		inline BraceBreakingStyle GetBraceBreakingStyle() const override
		{
			return m_config.braceBreakingStyle;
		}

		/*!
			Get the default include style.

			\return Include style.
		*/
		inline IncludeStyle GetIncludeStyle() const override
		{
			return m_config.includeStyle;
		}

		/*!
			Check whether the writer is on a new line and waiting for new
			chunks.

			\return True if the writer is on a new line
		*/
		inline bool IsOnNewLine() const override
		{
			return m_state == State::NewLine;
		}

		/*!
			Write an `std::string` chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		inline Writer& operator << (const std::string& chunk) override
		{
			if (m_state == State::NewLine)
			{
				if (!m_lineIndentCached.empty())
				{
					Write(m_lineIndentCached);
				}
				else
				{
					// allow writers to act on new lines

					Write("");
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
		inline Writer& operator << (const char* chunkString) override
		{
			return *this << std::string(chunkString);
		}

		/*!
			Write a new line chunk to the output.

			New line chunks can be configured with the \ref Config.

			\return Reference to itself to allow for chaining.
		*/
		inline Writer& operator << (const NextLine& command) override
		{
			(void)command;

			// edge-case for empty lines within a comment block

			if (m_isInCommentBlock &&
				m_lineChunkCountWritten == 0)
			{
				Write(" *");
			}

			WriteNewLine();

			m_state = State::NewLine;

			m_lineChunkCountWritten = 0;

			return *this;
		}

		/*!
			Increment the level of indentation.

			Indentation is applied only when the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		inline Writer& operator << (const IndentPush& command) override
		{
			(void)command;

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
		inline Writer& operator << (const IndentPop& command) override
		{
			(void)command;

			if (!m_isInCommentBlock)
			{
				if (m_lineIndentCount > 0)
				{
					CacheIndentation(
						m_lineIndentCached,
						--m_lineIndentCount
					);
				}
			}
			else
			{
				if (m_commentIndentCount > 0)
				{
					CacheIndentation(
						m_commentIndentCached,
						--m_commentIndentCount
					);
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
		inline Writer& operator << (Command&& command) override
		{
			command.Visit(*this);

			return *this;
		}

		/*!
			Set the writer to be inside a comment block, which will add " * "
			after the indentation of a new line.
		*/
		inline void SetIsInCommentBlock(bool value) override
		{
			m_isInCommentBlock = value;

			m_commentIndentCount = 0;
			m_commentIndentCached.clear();
		}

		/*!
			Check if the output was changed compared to what the implementation
			has seen before.
		*/
		inline virtual bool IsChanged() const override
		{
			return false;
		}

		/*!
			Commits the generated output to the target of a writer if the
			output was changed.

			\param force Force writing the file even if the output was not
			changed.

			\return Returns true if the commit was successful.
		*/
		inline bool Commit(bool force = false) override
		{
			return (force || IsChanged()) && OnCommit(force);
		}

	protected:
		/*!
			Writes a new line chunk to the output.
		*/
		void WriteNewLine() override
		{
			Write(m_config.chunkNewLine);
		}

	protected:
		TConfig m_config;

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

	//! \deprecated Prefer using \ref Writer instead.
	using WriterBase = Writer;

};
