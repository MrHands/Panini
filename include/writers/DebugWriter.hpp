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

#include "writers/WriterBase.hpp"

#ifdef _WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <Windows.h>
#endif

namespace panini
{

	constexpr size_t g_MinimumLinePadding = 3;
	constexpr int32_t g_MaxScreenHeight = 30;

	class DebugWriter
		: public WriterBase
	{

	public:
		enum Colors : uint16_t
		{
			eColors_Black    = 0x0000,
			eColors_Blue     = 0x0001,
			eColors_Green    = 0x0002,
			eColors_Red      = 0x0004,
			eColors_Yellow   = eColors_Red | eColors_Green,
			eColors_Cyan     = eColors_Green | eColors_Blue,
			eColors_Fuchsia  = eColors_Red | eColors_Blue,
			eColors_White    = eColors_Red | eColors_Green | eColors_Blue,
			eColors_Light    = 0x0008,
		};

		inline explicit DebugWriter()
		{
		#ifdef _WIN32
			m_output = ::GetStdHandle(STD_OUTPUT_HANDLE);
			m_intialized = ::GetConsoleScreenBufferInfo(m_output, &m_screenInfo);

			m_consoleWidth = static_cast<int32_t>(m_screenInfo.dwSize.X);
			m_consoleHeight = std::min(
				static_cast<int32_t>(m_screenInfo.dwSize.Y),
				g_MaxScreenHeight
			);

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

			for (int y = 0; y < m_consoleHeight - 1; ++y)
			{
				SetCursorPosition(0, y);
				std::cout << "----";
			}

			std::cout.flush();

			SetCursorPosition(0, m_consoleHeight - 1);
			std::cout << "> Press <Enter> to continue debugging, <Q> to quit";

			// reset cursor

			SetCursorPosition(0, 0);
		}

	private:
		inline virtual void Write(const std::string& chunk) override
		{
			// line numbers

			if (IsOnNewLine())
			{
				std::string padded = std::to_string(m_cursorY + 1);
				for (size_t i = padded.length(); i < g_MinimumLinePadding; ++i)
				{
					padded.insert(padded.begin(), ' ');
				}
				
				SetColor(eColors_White, eColors_Black);
				WriteChunk(padded + " ");
				ResetStyles();
			}

			const std::string& indentStr = GetConfig().chunkIndent;

			// new line

			if (chunk == GetConfig().chunkNewLine)
			{
				SetColor(eColors_Fuchsia, eColors_White);
				WriteChunk("<LF>");
				ResetStyles();

				SetCursorPosition(0, m_cursorY + 1);
			}

			// indentation

			else if (
				chunk.substr(0, indentStr.length()) == indentStr)
			{
				SetColor(eColors_Green, eColors_White);

				size_t offset = 0;
				while (chunk.substr(offset, indentStr.length()) == indentStr)
				{
					WriteChunk(">>> ");

					offset += indentStr.length();
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
			return true;
		}

		inline void SetCursorPosition(int32_t x, int32_t y)
		{
			m_cursorX = x;
			m_cursorY = y;

		#ifdef _WIN32
			m_cursor.X = m_cursorX;
			m_cursor.Y = m_cursorY;
			::SetConsoleCursorPosition(m_output, m_cursor);
		#endif
		}

		inline void SetColor(uint16_t background, uint16_t foreground)
		{
		#ifdef _WIN32
			::SetConsoleTextAttribute(m_output, (background << 4) | foreground);
		#endif
		}

		inline void ResetStyles()
		{
			SetColor(eColors_Black, eColors_White);
		}

		inline void WriteChunk(const std::string& chunk)
		{
			if (m_cursorX + chunk.length() >= m_consoleWidth)
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

	private:
		bool m_intialized = false;
		int32_t m_cursorX = 0;
		int32_t m_cursorY = 0;
		int32_t m_consoleWidth = 0;
		int32_t m_consoleHeight = 0;

	#ifdef _WIN32
		HANDLE m_output;
		CONSOLE_SCREEN_BUFFER_INFO m_screenInfo;
		COORD m_cursor = { 0, 0 };
	#endif

	};

};
