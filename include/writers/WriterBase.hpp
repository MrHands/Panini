#pragma once

#include "commands/CommandBase.hpp"
#include "commands/IndentPop.hpp"
#include "commands/IndentPush.hpp"
#include "commands/NextLine.hpp"
#include "Config.hpp"

namespace panini {

	class WriterBase
	{

	public:
		explicit WriterBase(const Config& config = Config())
			: m_config(config)
		{
			m_indentCached.reserve(8 * m_config.chunkIndent.size());
		}

		virtual ~WriterBase() = default;

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

		WriterBase& operator << (const NextLine& command)
		{
			Write(m_config.chunkNewLine);

			m_state = State::NewLine;

			return *this;
		}

		WriterBase& operator << (const IndentPush& command)
		{
			m_indentCount++;

			CacheIndentation();

			return *this;
		}

		WriterBase& operator << (const IndentPop& command)
		{
			if (m_indentCount > 0)
			{
				m_indentCount--;

				CacheIndentation();
			}

			return *this;
		}

		WriterBase& operator << (CommandBase&& command)
		{
			command.Visit(*this);

			return *this;
		}

		bool IsOnNewLine() const
		{
			return m_state == State::NewLine;
		}

		BraceBreakingStyle GetBraceStyle() const
		{
			return m_config.braces;
		}

	protected:
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

	protected:
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
