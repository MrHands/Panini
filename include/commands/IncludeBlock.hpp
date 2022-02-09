/*
	MIT No Attribution

	Copyright 2021-2022 Mr. Hands

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to
	deal in the Software without restriction, including without limitation the
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "commands/CommandBase.hpp"
#include "commands/Include.hpp"
#include "data/IncludeSet.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Outputs a block of include statements.

		Example:

		\code{.cpp}
			IncludeSet set;
			set.Add("game/systems/Audio.h");
			set.Add("game/Physics.h");
			set.Add("game/systems/Particles.h");
			set.Add("stdio.h", IncludeStyle::AngularBrackets);

			writer << IncludeBlock(set);
		\endcode

		Output:

		\code{.cpp}
			#include <stdio.h>
			#include "game/systems/Audio.h"
			#include "game/systems/Particles.h"
			#include "game/Physics.h"
		\endcode
	*/
	class IncludeBlock
		: public CommandBase
	{

	public:
		/*!
			Construct an IncludeBlock command from an IncludeSet that is
			copied to the instance.
		*/
		inline IncludeBlock(const IncludeSet& set)
			: m_set(set)
		{
		}

		/*!
			Construct an IncludeBlock command from an IncludeSet that is
			moved into the instance.
		*/
		inline IncludeBlock(IncludeSet&& set) noexcept
			: m_set(std::exchange(set, {}))
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			// sort includes and resolve "inherit" include style with the
			// writer's include style

			m_set.Sort(writer.GetIncludeStyle());

			// write includes

			size_t includeIndex = 0;
			for (IncludeEntry& entry : m_set)
			{
				if (includeIndex++ > 0)
				{
					writer << NextLine();
				}

				writer << Include(entry);
			}
		}

	private:
		IncludeSet m_set;

	};

};
