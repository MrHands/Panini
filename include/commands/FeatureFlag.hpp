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
		\brief Command for feature flags.

		\ingroup Commands

		When the condition is true, the "then" callback is used to output code
		to the active writer. Otherwise, if the "else" callback is specified,
		this callback is used instead.

		If the condition was true, the output is wrapped in single-line comments
		to make the generated code stand out more. These comments are omitted if
		the `context` parameter is left blank.

		Feature flags are useful when you are working on generated code that
		depends on the context. For example, you have developed a new way to
		access the main database, but want to roll out this change in controlled
		stages instead of simultaneously.

		Example:

		\code{.cpp}
			std::string query = R"(INSERT INTO Users (name) VALUES ('Little Bobby Tables'))";
			writer << FeatureFlag(
				newDbAccess,
				"new-db-access",
				[&query](Writer& writer) {
					writer << "db->WriteQueryFast(\"" << query << ";\");" << NextLine();
				},
				[&query](Writer& writer) {
					writer << "db->WriteQuery(\"" << query << ";\");" << NextLine();
				}
			);
		\endcode

		Output when the condition is true:

		\code{.cpp}
			// new-db-access
			db->WriteQueryFast("INSERT INTO Users (name) VALUES ('Little Bobby Tables');");
			// new-db-access
		\endcode

		Output when the condition is false:

		\code{.cpp}
			db->WriteQuery("INSERT INTO Users (name) VALUES ('Little Bobby Tables');");
		\endcode
	*/

	class FeatureFlag
		: public Command
	{

	public:
		/*!
			Callback type used for conditions.

			\param writer     Active writer.
		*/
		using TCallback = std::function<void(Writer& writer)>;

		/*!
			Construct a FeatureFlag with a callback invoked when the condition
			is true.

			\param condition     Decide which callback to invoke
			\param context       String added to comments for context
			\param callbackThen  Invoked when condition is true
		*/
		inline explicit FeatureFlag(
			bool condition,
			const std::string& context,
			TCallback&& callbackThen) noexcept
			: m_condition(condition)
			, m_context(context)
			, m_callbackThen(std::move(callbackThen))
		{
		}

		/*!
			Construct a FeatureFlag with a callback invoked when the condition
			is true and one when the condition is false.

			\param condition     Decide which callback to invoke
			\param context       String added to comments for context
			\param callbackThen  Invoked when condition is true
			\param callbackElse  Invoked when condition is false
		*/
		inline explicit FeatureFlag(
			bool condition,
			const std::string& context,
			TCallback&& callbackThen,
			TCallback&& callbackElse) noexcept
			: m_condition(condition)
			, m_context(context)
			, m_callbackThen(std::move(callbackThen))
			, m_callbackElse(std::move(callbackElse))
		{
		}

		inline void Visit(Writer& writer) override
		{
			if (m_condition)
			{
				if (!m_context.empty())
				{
					writer << CommentLine(m_context) << NextLine();
				}

				m_callbackThen(writer);
				
				if (!m_context.empty())
				{
					writer << CommentLine(m_context);
				}
			}
			else if (
				m_callbackElse)
			{
				m_callbackElse(writer);
			}
		}

	private:
		bool m_condition = false;
		std::string m_context;
		TCallback m_callbackThen;
		TCallback m_callbackElse;

	};

};
