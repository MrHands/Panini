#pragma once

#include <iostream>

#include "writers/WriterBase.hpp"

namespace panini
{

	class ConsoleWriter
		: public WriterBase
	{

	public:
		virtual void Write(const std::string& chunk) final
		{
			std::cout << chunk;
		}

	};

};
