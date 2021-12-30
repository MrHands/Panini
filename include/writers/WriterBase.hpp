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

		Writers take chunks and commands as input and process them to output. You can
		configure them with a @ref Config instance.
	*/
	class WriterBase
	{

	public:
		/*!
			Constructs a writer with an optional configuration instance.
		*/
		explicit WriterBase(const Config& config = Config())
			: m_config(config)
		{
			m_indentCached.reserve(8 * m_config.chunkIndent.size());

			// inherit is not allowed as the brace breaking style on the config

			if (m_config.braces == BraceBreakingStyle::Inherit)
			{
				m_config.braces = BraceBreakingStyle::Allman;
			}
		}

		virtual ~WriterBase() = default;

		/*!
			Write an `std::string` chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		WriterBase& operator << (const std::string& chunk)
		{
			if (m_state == State::NewLine)
			{
				if (!m_indentCached.empty())
				{
					Write(m_indentCached);
				}

				m_state = State::Chunk;
			}

			Write(chunk);

			return *this;
		}

		/*!
			Write a C-style string chunk to the output.

			Will add indentation if the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/

		WriterBase& operator << (const char* chunkString)
		{
			return *this << std::string(chunkString);
		}

		/*!
			Write a new line chunk to the output.

			New line chunks can be configured with the @ref Config.

			\return Reference to itself to allow for chaining.
		*/
		WriterBase& operator << (const NextLine& command)
		{
			Write(m_config.chunkNewLine);

			m_state = State::NewLine;

			return *this;
		}

		/*!
			Increment the level of indentation.

			Indentation is applied only when the writer is on a new line.

			\return Reference to itself to allow for chaining.
		*/
		WriterBase& operator << (const IndentPush& command)
		{
			m_indentCount++;

			CacheIndentation();

			return *this;
		}

		/*!
			Decrement the level of indentation.

			Indentation is applied when the writer is on a new line. The level
			of indentation cannot go negative.

			\return Reference to itself to allow for chaining.
		*/
		WriterBase& operator << (const IndentPop& command)
		{
			if (m_indentCount > 0)
			{
				m_indentCount--;

				CacheIndentation();
			}

			return *this;
		}

		/*!
			Apply a command.

			\warning Commands are moved instead of copied!

			Visit a command instance and allow it to modify the output.

			\return Reference to itself to allow for chaining.
		*/
		WriterBase& operator << (CommandBase&& command)
		{
			command.Visit(*this);

			return *this;
		}

		/*!
			Check whether the writer is currently on a new line.

			\return True if the writer is on a new line.
		*/
		bool IsOnNewLine() const
		{
			return m_state == State::NewLine;
		}

		/*!
			Get the current brace breaking style.

			\return Brace breaking style.
		*/
		BraceBreakingStyle GetBraceStyle() const
		{
			return m_config.braces;
		}

	protected:
		/*!
			Interface method for writing chunks to the output.
		*/
		virtual void Write(const std::string& chunk) = 0;

	private:
		void CacheIndentation()
		{
			m_indentCached.clear();

			for (int32_t i = 0; i < m_indentCount; ++i)
			{
				m_indentCached += m_config.chunkIndent;
			}
		}

	private:
		Config m_config;

		int32_t m_indentCount = 0;
		std::string m_indentCached;

		enum class State
		{
			NewLine,
			Chunk
		};
		State m_state = State::NewLine;

	};

};
