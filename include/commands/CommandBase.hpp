#pragma once

#include "Config.hpp"

namespace panini
{

	class WriterBase;

	/*!
		\brief Base class for commands.

		Commands are used to output chunks to the writer. They should be
		designed to be chained together. Because commands cannot undo output
		that was already written, you should be careful not to output more than
		you strictly need. A good rule of thumb is that a command should never
		end on a \ref NextLine command.

		\sa WriterBase
	*/
	class CommandBase
	{

	public:
		virtual ~CommandBase() = default;

		/*!
			Accepts a \ref WriterBase to write chunks to the output.
		*/
		virtual void Visit(WriterBase& writer) = 0;

	};

};
