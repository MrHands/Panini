#pragma once

#include "Config.hpp"

namespace panini
{

	class WriterBase;

	/*!
		\brief Base class for commands.

		Commands are used to output chunks to the writer. They should be
		designed to be chained together. Because commands cannot undo output
		that was already written, you should be careful to not output more than
		you strictly need. A good rule of thumb is that a command should never
		end on a \ref NewLine.

		\sa WriterBase
	*/
	class CommandBase
	{

	public:
		virtual ~CommandBase() = default;

		/*!
			Accepts a \ref WriterBase that is used for writing chunks to the
			output.
		*/
		virtual void Visit(WriterBase& writer) = 0;

	};

};
