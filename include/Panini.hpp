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

// C/STL

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

// Global

/*!
	\defgroup Globals
	\defgroup Commands
	\defgroup CommandOptions
	\defgroup Writers
	\defgroup WriterConfiguration
	\defgroup Data
*/

#include "Version.hpp"

// Commands

#include "commands/Braces.hpp"
#include "commands/CommaList.hpp"
#include "commands/CommentBlock.hpp"
#include "commands/CommentLine.hpp"
#include "commands/FeatureFlag.hpp"
#include "commands/Include.hpp"
#include "commands/IncludeBlock.hpp"
#include "commands/IndentPop.hpp"
#include "commands/IndentPush.hpp"
#include "commands/Label.hpp"
#include "commands/NextLine.hpp"
#include "commands/Scope.hpp"

// Writers

#include "writers/CompareWriter.hpp"
#include "writers/ConsoleWriter.hpp"
#include "writers/DebugWriter.hpp"
#include "writers/FileWriter.hpp"
#include "writers/StringWriter.hpp"
