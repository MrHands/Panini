#pragma once

#include "commands/IndentPop.hpp"
#include "commands/IndentPush.hpp"
#include "commands/NextLine.hpp"
#include "Config.hpp"

namespace panini {

	class WriterBase
	{

	public:
		WriterBase(const Config& config = Config())
			: m_config(config)
		{}
		virtual ~WriterBase() = default;

		WriterBase& operator << (const std::string& chunk)
		{
			if (m_state == State::NewLine)
			{
				for (int32_t i = 0; i < m_indentCount; ++i)
				{
					Write(m_config.chunkIndent);
				}

				m_state = State::Chunk;
			}

			Write(chunk);

			return *this;
		}

		WriterBase& operator << (const NextLine& command)
		{
			Write(m_config.chunkNewLine);

			return *this;
		}

		WriterBase& operator << (const IndentPush& command)
		{
			m_indentCount++;

			return *this;
		}

		WriterBase& operator << (const IndentPop& command)
		{
			if (m_indentCount > 0)
			{
				m_indentCount--;
			}

			return *this;
		}

	protected:
		virtual void Write(const std::string& chunk) = 0;

	protected:
		Config m_config;

		int32_t m_indentCount = 0;

		enum class State
		{
			NewLine,
			Chunk
		};
		State m_state = State::NewLine;

	};

};
