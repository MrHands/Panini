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

#include "commands/Braces.hpp"
#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Output a scope with braces.

		Scopes have a named chunk and use a \ref Braces command to add opening
		and closing braces to the output.

		Note that the command will automatically add a space between the name
		and the opening brace if the brace breaking style was set to
		\ref BraceBreakingStyle::Attach, either by the writer or as an override
		for this command.

		Example:

		\code{.cpp}
			writer << Scope("void SpaceShip::Fire()", [](WriterBase& writer) {
				writer << Scope(R"(g_AudioManager->PlaySound("pewpew.wav", [this](Sound& sound))", [](WriterBase& writer) {
					writer << "FireMissile();" << NextLine();
				}, BraceBreakingStyle::Attach);
			});
		\endcode

		Output:

		\code{.cpp}
			void SpaceShip::Fire()
			{
				g_AudioManager->PlaySound("pewpew.wav", [this](Sound& sound) {
					FireMissile();
				}
			}
		\endcode

		\sa Braces
	*/
	class Scope
		: public CommandBase
	{

	public:
		/*!
			Create a Scope with a `name` and a `callback` that are moved into
			the instance.

			The callback is called when the command is visited by a
			\ref WriterBase.
			
			Setting the `style` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Scope(std::string&& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle style = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(callback)
			, m_style(style)
		{
		}

		/*!
			Create a Scope with a `name` that is copied and a `callback` that is
			moved into the instance.

			The callback is called when the command is visited by a
			\ref WriterBase.

			Setting the `style` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Scope(const std::string& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle braceStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(callback)
			, m_style(braceStyle)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			BraceBreakingStyle braceStyle =
				m_style == BraceBreakingStyle::Inherit
					? writer.GetBraceBreakingStyle()
					: m_style;

			if (!m_name.empty())
			{
				writer << m_name;

				if (braceStyle == BraceBreakingStyle::Attach)
				{
					writer << " ";
				}
			}

			writer << Braces(std::move(m_callback), braceStyle);
		}

	private:
		std::string m_name;
		std::function<void(WriterBase&)> m_callback;
		BraceBreakingStyle m_style;

	};

};
