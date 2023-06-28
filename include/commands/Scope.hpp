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

#include "commands/Braces.hpp"
#include "commands/Command.hpp"
#include "options/ScopeOptions.hpp"
#include "writers/Writer.hpp"

namespace panini
{

	/*!
		\brief Command for outputting a scope with braces.

		Scopes have a named chunk and use a \ref Braces command to add opening
		and closing braces to the output.

		Note that the command will automatically add a chunk between the name
		and the opening brace if the brace breaking style was set to
		\ref BraceBreakingStyle::Attach by the writer or as an override for this
		command. This chunk can be configured with \ref ScopeOptions.

		Example:

		\code{.cpp}
			writer << Scope("void SpaceShip::Fire()", [](Writer& writer) {
				writer << Scope(R"(g_AudioManager->PlaySound("pewpew.wav", [this](Sound& sound))", [](Writer& writer) {
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
		: public Command
	{

	public:
		using TCallback = std::function<void(Writer&)>;

		/*!
			\deprecated This constructor will be removed in the next major
			release. Prefer using the constructor that takes \ref ScopeOptions

			Create a Scope with a `name` and a `callback` that are both moved
			into the instance.

			The callback is called when the command is visited by a \ref Writer.
			
			Setting the `breakingStyle` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Scope(
			std::string&& name,
			TCallback&& callback,
			BraceBreakingStyle breakingStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(std::move(callback))
		{
			m_options.breakingStyle = breakingStyle;
		}

		/*!
			\deprecated This constructor will be removed in the next major
			release. Prefer using the constructor that takes \ref ScopeOptions

			Create a Scope with a `name` that is copied and a `callback` that is
			moved into the instance.

			The callback is called when the command is visited by a
			\ref Writer.

			Setting the `breakingStyle` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Scope(
			const std::string& name,
			TCallback&& callback,
			BraceBreakingStyle breakingStyle = BraceBreakingStyle::Inherit)
			: m_name(name)
			, m_callback(std::move(callback))
		{
			m_options.breakingStyle = breakingStyle;
		}

		/*!
			Create a Scope with a `name` and a `callback` that are both moved
			into the instance. `options` are used to configure the command
			instance.
		*/
		inline Scope(
			std::string&& name,
			TCallback&& callback,
			const ScopeOptions& options)
			: m_name(name)
			, m_callback(std::move(callback))
			, m_options(options)
		{
		}

		/*!
			Create a Scope with a `name` that is copied and a `callback` that is
			moved into the instance. `options` are used to configure the
			command instance.
		*/
		inline Scope(
			const std::string& name,
			TCallback&& callback,
			const ScopeOptions& options)
			: m_name(name)
			, m_callback(std::move(callback))
			, m_options(options)
		{
		}

		inline void Visit(Writer& writer) final
		{
			const BraceBreakingStyle breakingStyle =
				m_options.breakingStyle == BraceBreakingStyle::Inherit
					? writer.GetBraceBreakingStyle()
					: m_options.breakingStyle;

			if (!m_name.empty())
			{
				writer << m_name;

				if (breakingStyle == BraceBreakingStyle::Attach)
				{
					writer << m_options.chunkAttachSpacing;
				}
			}

			writer << Braces(std::move(m_callback), m_options);
		}

	private:
		std::string m_name;
		TCallback m_callback;
		ScopeOptions m_options;

	};

};
