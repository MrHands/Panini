#pragma once

#include "Config.hpp"

namespace panini
{

	class WriterBase;

	class CommandBase
	{

	public:
		virtual ~CommandBase() = default;

		virtual void Visit(WriterBase& writer) = 0;

	};

};
