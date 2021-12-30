#pragma once

#include "commands/Braces.hpp"
#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Output a scope with braces.

		Scopes have a named chunk and opening curly braces. You can override
		the \ref BraceBreakingStyle defined in the writer when you create an
		instance.

		Note that the command will *not* automatically add a space between the
		name and the opening brace if the brace breaking style was set to
		\ref BraceBreakingStyle::Attach.

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
		Scope(std::string&& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle style = BraceBreakingStyle::Inherit)
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
		Scope(const std::string& name, std::function<void(WriterBase&)>&& callback, BraceBreakingStyle braceStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(callback)
			, m_style(braceStyle)
		{
		}

		virtual void Visit(WriterBase& writer) final
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
