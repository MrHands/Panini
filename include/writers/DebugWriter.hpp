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

#ifdef _WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <Windows.h>
#endif

#include "writers/WriterBase.hpp"

namespace panini
{

	/*!
		Configuration for the \ref DebugWriter class.
	*/

	struct DebugWriterConfig
		: public Config
	{
		/*!
			Minimum length of line numbers, padded with spaces.
		*/
		size_t lineNumberPadding = 3;
	};

	/*!
		\brief Writes output to the console line-by-line.

		The DebugWriter will halt execution every time it receives a
		\ref NextLine command. The writer will wait for user input to either
		continue debugging or stop execution.

		It will display line numbers before the output and highlight both
		indentation and new lines in the output.

		The \ref DebugWriterConfig instance is used to configure the output.
	*/

	class DebugWriter
		: public WriterBase
	{

	public:
		/*!
			Colors used in the console output.
		*/
		struct Colors
		{
			enum Values : uint16_t
			{
				Black    = 0,
				Blue     = (1 << 0),
				Green    = (1 << 1),
				Red      = (1 << 2),
				Yellow   = Red | Green,
				Cyan     = Green | Blue,
				Fuchsia  = Red | Blue,
				White    = Red | Green | Blue,
				Light    = (1 << 3),
			};
		};

		/*!
			Construct and configure the writer.
		*/
		inline explicit DebugWriter(const DebugWriterConfig& config = DebugWriterConfig{})
			: WriterBase(config)
			, m_debugConfig(config)
		{
		#ifdef _WINDOWS
			m_output = ::GetStdHandle(STD_OUTPUT_HANDLE);
			m_initialized = ::GetConsoleScreenBufferInfo(m_output, &m_screenInfo);

			m_consoleWidth = static_cast<int32_t>(m_screenInfo.dwSize.X);
			m_consoleHeight = static_cast<int32_t>(m_screenInfo.dwSize.Y);

			const DWORD screenSize = m_screenInfo.dwSize.X * m_screenInfo.dwSize.Y;

			// clear the screen with spaces

			DWORD written = 0;
			::FillConsoleOutputCharacterA(
				m_output,
				' ',
				screenSize,
				m_cursor,
				&written
			);

			// clear background formatting

			::FillConsoleOutputAttribute(
				m_output,
				m_screenInfo.wAttributes,
				screenSize,
				m_cursor,
				&written
			);
		#endif

			std::cout.flush();

			// reset cursor

			SetCursorPosition(0, 0);
		}

		/*!
			Always handle remaining input when \ref Commit is called.
		*/
		inline virtual bool IsChanged() const override
		{
			return true;
		}

	protected:
		/*!
			Writes the chunk to the console.
		*/
		inline virtual void Write(const std::string& chunk) override
		{
			if (!m_initialized)
			{
				return;
			}

			// line numbers

			if (IsOnNewLine())
			{
				std::string padded = std::to_string(m_cursorY + 1);
				for (size_t i = padded.length(); i < m_debugConfig.lineNumberPadding; ++i)
				{
					padded.insert(padded.begin(), ' ');
				}
				
				SetColor(Colors::White, Colors::Black);
				WriteChunk(padded + " ");
				ResetStyles();
			}

			const std::string& indentStr = GetConfig().chunkIndent;

			// new line

			if (chunk == GetConfig().chunkNewLine)
			{
				SetColor(Colors::Fuchsia, Colors::White);
				WriteChunk("<LF>");
				ResetStyles();

				SetCursorPosition(0, m_cursorY + 1);

				// handle input

				if (m_isDebugging)
				{
					HandleInput("Press <Enter> to continue debugging, <Q> to quit");
				}
			}

			// indentation

			else if (
				chunk.substr(0, indentStr.length()) == indentStr)
			{
				size_t offset = 0;
				int32_t count = 0;

				while (chunk.substr(offset, indentStr.length()) == indentStr)
				{
					SetColor(
						(count % 2 == 0) ? Colors::Yellow : Colors::Cyan,
						Colors::White
					);
					WriteChunk("-> ");

					offset += indentStr.length();
					count++;
				}

				ResetStyles();
			}

			// other chunks

			else
			{
				WriteChunk(chunk);
			}
		}

		inline virtual bool OnCommit(bool force = false) override
		{
			HandleInput("<END>");

			SetCursorPosition(0, m_cursorY + 1);

			return true;
		}

		/*!
			Moves the cursor in the console window.
		*/
		inline void SetCursorPosition(int32_t x, int32_t y)
		{
			m_cursorX = x;
			m_cursorY = y;

		#ifdef _WINDOWS
			m_cursor.X = m_cursorX;
			m_cursor.Y = m_cursorY;
			::SetConsoleCursorPosition(m_output, m_cursor);
		#endif
		}

		/*!
			Sets the background and foreground colors used for the output.
		*/
		inline void SetColor(uint16_t background, uint16_t foreground)
		{
		#ifdef _WINDOWS
			::SetConsoleTextAttribute(m_output, (background << 4) | foreground);
		#else
			(void)background;
			(void)foreground;
		#endif
		}

		/*!
			Reset colors to the default white on black color scheme.
		*/
		inline void ResetStyles()
		{
			SetColor(Colors::Black, Colors::White);
		}

		/*!
			Internal method for writing a chunk to the console window, clipping
			it and pushing it to the next line if it's too long.
		*/
		inline void WriteChunk(const std::string& chunk)
		{
			if (m_cursorX + chunk.length() > m_consoleWidth)
			{
				WriteChunk(chunk.substr(0, m_consoleWidth - chunk.length()));

				SetCursorPosition(0, m_cursorY + 1);
				WriteChunk(chunk.substr(m_consoleWidth - chunk.length()));

				return;
			}

			std::cout.flush();
			std::cout << chunk;

			SetCursorPosition(
				m_cursorX + chunk.length(),
				m_cursorY
			);
		}

		/*!
			Handle user input.
		*/
		inline void HandleInput(const std::string& message)
		{
			SetCursorPosition(0, m_cursorY + 1);
			SetColor(Colors::White | Colors::Light, Colors::Black);
			std::cout << message;
			ResetStyles();

			// wait for input

		#ifdef _WINDOWS
			HANDLE input = ::GetStdHandle(STD_INPUT_HANDLE);

			while (1)
			{
				INPUT_RECORD record;
				DWORD read = 0;
				::ReadConsoleInputA(input, &record, 1, &read);
				if (record.EventType == KEY_EVENT)
				{
					KEY_EVENT_RECORD& keyEvent = record.Event.KeyEvent;

					// wait for the key to be released

					if (keyEvent.bKeyDown)
					{
						continue;
					}

					// stop debugging when Q is pressed

					m_isDebugging = keyEvent.wVirtualKeyCode != 'Q';

					break;
				}
			}
		#endif

			// clear line

		#ifdef _WINDOWS
			DWORD written = 0;
			::FillConsoleOutputCharacterA(
				m_output,
				' ',
				message.length(),
				m_cursor,
				&written
			);

			::FillConsoleOutputAttribute(
				m_output,
				m_screenInfo.wAttributes,
				message.length(),
				m_cursor,
				&written
			);
		#endif

			// reset cursor

			SetCursorPosition(0, m_cursorY - 1);
		}

	protected:
		DebugWriterConfig m_debugConfig;
		bool m_initialized = false;
		bool m_isDebugging = true;
		int32_t m_cursorX = 0;
		int32_t m_cursorY = 0;
		int32_t m_consoleWidth = 0;
		int32_t m_consoleHeight = 0;

	#ifdef _WINDOWS
		HANDLE m_output;
		CONSOLE_SCREEN_BUFFER_INFO m_screenInfo;
		COORD m_cursor = { 0, 0 };
	#endif

	};

};
