#pragma once

#include "commands/CommandBase.hpp"
#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		\brief Outputs opening and closing curly braces.

		Adding opening and closing braces to the output can be frustrating
		to do by hand because you have to take the \ref BraceBreakingStyle and
		indentation into account. This command handles that for you.

		If you want to prefix the opening brace with a chunk, it may be
		beneficial to use a \ref Scope instead.

		Note that the command will *not* output a space before the opening
		brace if the brace breaking style was set to
		\ref BraceBreakingStyle::Attach

		Example:

		\code{.cpp}
			writer << Braces([](WriterBase& writer) {
				writer << "const char* passwords[] = " << Braces([](WriterBrace& writer) {
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
		: public CommandBase
	{

	public:
		/*!
			Create a Braces command a `callback` that is moved into the
			instance.

			The callback is called when the command is visited by a
			\ref WriterBase.

			Setting the `style` parameter to \ref BraceBreakingStyle::Inherit
			copies the brace breaking style from the writer, otherwise it will
			be overridden for this command only.
		*/
		inline Braces(std::function<void(WriterBase&)>&& callback, BraceBreakingStyle braceStyle = BraceBreakingStyle::Inherit)
			: m_callback(callback)
			, m_braceStyle(braceStyle)
		{
		}

		inline virtual void Visit(WriterBase& writer) final
		{
			BraceBreakingStyle braceStyle =
				m_braceStyle == BraceBreakingStyle::Inherit
					? writer.GetBraceBreakingStyle()
					: m_braceStyle;

			const bool wasNewLine = writer.IsOnNewLine();

			switch (braceStyle)
			{

			case BraceBreakingStyle::Attach:
				{
					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Allman:
				{
					if (!wasNewLine)
					{
						writer << NextLine();
					}

					writer << "{" << IndentPush() << NextLine();
					m_callback(writer);
					writer << IndentPop() << "}";

				} break;

			case BraceBreakingStyle::Whitesmiths:
				{
					if (!wasNewLine)
					{
						writer << NextLine() << IndentPush();
					}

					writer << "{" << NextLine();
					m_callback(writer);
					writer << "}";

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
		std::function<void(WriterBase&)> m_callback;
		BraceBreakingStyle m_braceStyle;

	};

};
