#pragma once

#include "WriterBase.hpp"

namespace panini
{
	class StringWriter
		: public WriterBase
	{

	public:
		StringWriter(std::string& target, const Config& config = Config())
			: WriterBase(config)
			, m_target(target)
		{
		}

	private:
		virtual void Write(const std::string& chunk) final
		{
			m_target += chunk;
		}

	private:
		std::string& m_target;

	};
};
