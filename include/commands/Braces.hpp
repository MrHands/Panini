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

#include "commands/Command.hpp"
#include "options/BracesOptions.hpp"
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Command for outputting opening and closing (curly) braces.

		This command tracks the current indentation level and takes the brace
		breaking style into account when outputting braces. You can change both
		the brace breaking style and the brace chunks it outputs with the
		\ref BracesOptions parameter.

		After outputting the opening brace, the Braces command calls a callback
		with the current \ref Writer instance as a parameter.

		If you want to prefix the opening brace with a chunk, it may be
		beneficial to use a \ref Scope instead.

		\note The command will *not* output a space before the opening brace if
		the brace breaking style was set to BraceBreakingStyle::Attach

		Example:

		\code{.cpp}
			writer << Braces([](Writer& writer) {
				writer << "const char* passwords[] = " << Braces([](Writer& writer) {
					writer << R"("password",)" << NextLine();
					writer << R"("p4ssw0rd")" << NextLine();
				}, BraceBreakingStyle::Attach) << ";" << NextLine();
			});
		\endcode

		Output:

		\code{.cpp}
			{
				const char* passwords[] = {
					"password",
					"p4ssw0rd"
				};
			}
		\endcode

		\sa Scope
	*/
	class Braces
		: public Command
	{

	public:
		using TCallback = std::function<void(Writer&)>;

		/*!
			Create a Braces command with a callback that is moved into the
			instance.

			The callback is called when the command is visited by a
			\ref Writer.

			Setting the `breakingStyle` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Braces(
			TCallback&& callback,
			BraceBreakingStyle breakingStyle = BraceBreakingStyle::Inherit)
			: m_callback(std::move(callback))
		{
			m_options.breakingStyle = breakingStyle;
		}

		/*!
			Create a Braces command a `callback` that is moved into the
			instance.

			The callback is called when the command is visited by a
			\ref Writer.

			Setting the `breakingStyle` parameter in the options to
			\ref BraceBreakingStyle::Inherit copies the brace breaking style
			from the writer, otherwise it will be overridden for this command
			only.
		*/
		inline Braces(TCallback&& callback, const BracesOptions& options)
			: m_callback(std::move(callback))
			, m_options(options)
		{
		}

		inline void Visit(Writer& writer) final
		{
			const BraceBreakingStyle breakingStyle =
				m_options.breakingStyle == BraceBreakingStyle::Inherit
					? writer.GetBraceBreakingStyle()
					: m_options.breakingStyle;

			const bool wasNewLine = writer.IsOnNewLine();

			switch (breakingStyle)
			{

			case BraceBreakingStyle::Attach:
				{
					writer << m_options.chunkBraceOpen << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << m_options.chunkBraceClose;

				} break;

			case BraceBreakingStyle::Allman:
				{
					if (!wasNewLine)
					{
						writer << NextLine();
					}

					writer << m_options.chunkBraceOpen << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << m_options.chunkBraceClose;

				} break;

			case BraceBreakingStyle::Whitesmiths:
				{
					if (!wasNewLine)
					{
						writer << NextLine() << IndentPush();
					}

					writer << m_options.chunkBraceOpen << NextLine();
					m_callback(writer);
					writer << m_options.chunkBraceClose;

					if (!wasNewLine)
					{
						writer << IndentPop();
					}

				} break;

			default:
				break;

			}
		}

	private:
		TCallback m_callback;
		BracesOptions m_options;

	};

};
