#pragma once

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
			Write(chunk);

			return *this;
		}

	protected:
		virtual void Write(const std::string& chunk) = 0;

	protected:
		Config m_config;

	};

};