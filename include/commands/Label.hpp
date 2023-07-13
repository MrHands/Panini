/*
	MIT No Attribution

	Copyright 2021-2023 Mr. Hands

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

#include "commands/Command.hpp"

namespace panini
{

	/*!
		\brief Command for outputting a label statement.

		A label is a name and a ":" chunk. The command pops the indentation
		before writing the label and restores it afterwards.

		Labels are useful when you don't want to modify the current
		indentation level, e.g. when writing an access identifier for a class
		or a switch..case statement.

		Example:

		\code{.cpp}
			writer << Scope("class Vehicle", [](Writer& writer) {
				writer << Label("public") << NextLine();
				writer << "Vehicle(const std::string& maker);" << NextLine();
				writer << Label("private") << NextLine();
				writer << "std::string m_maker;" << NextLine();
			}) << ";";
		\endcode

		Output:

		\code{.cpp}
			class Vehicle
			{
			public:
				Vehicle(const std::string& maker);
			private:
				std::string m_maker;
			};
		\endcode

		\sa Scope
	*/
	class Label
		: public Command
	{

	public:
		/*!
			Create a Label command with a `name` that is moved into the
			instance.
		*/
		Label(std::string&& name)
			: m_name(name)
		{
		}

		/*!
			Create a Label command with a `name` that is copied to the
			instance.
		*/
		Label(const std::string& name)
			: m_name(name)
		{
		}

		virtual void Visit(Writer& writer) final
		{
			writer << IndentPop() << m_name << ":" << IndentPush();
		}

	private:
		std::string m_name;

	};

};
