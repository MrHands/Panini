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

	class FeatureFlag
		: public Command
	{

	public:
		using TCallback = std::function<void(Writer&)>;

		inline FeatureFlag(
			bool condition,
			const std::string& comment,
			TCallback&& callbackThen)
			: m_condition(condition)
			, m_comment(comment)
			, m_callbackThen(std::move(callbackThen))
		{
		}

		inline FeatureFlag(
			bool condition,
			const std::string& comment,
			TCallback&& callbackThen,
			TCallback&& callbackElse)
			: m_condition(condition)
			, m_comment(comment)
			, m_callbackThen(std::move(callbackThen))
			, m_callbackElse(std::move(callbackElse))
		{
		}

		inline void Visit(Writer& writer) override
		{
			if (m_condition)
			{
				writer << CommentLine(m_comment) << NextLine();

				m_callbackThen(writer);

				writer << CommentLine(m_comment);
			}
			else if (
				m_callbackElse)
			{
				m_callbackElse(writer);
			}
		}

	private:
		bool m_condition = false;
		std::string m_comment;
		TCallback m_callbackThen;
		TCallback m_callbackElse;

	};

};
