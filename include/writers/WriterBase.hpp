#pragma once

#include "Config.hpp"

namespace panini {

	class WriterBase
	{

	public:
		WriterBase(Config&& config)
			: m_config(config)
		{}
		virtual ~WriterBase() = default;

		virtual void Write(const std::string& chunk) = 0;

	protected:
		Config m_config;

	};

};