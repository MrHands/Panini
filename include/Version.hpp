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

/*!
	Major version number for the library.

	This number indicates that the library changed in an incompatible way with
	your existing code.

	For example, code built using Panini 1.x.x will not be compatible with
	version 2.x.x and will have to be rewritten.

	\ingroup Globals
*/
#define PANINI_VERSION_MAJOR 1

/*!
	Minor version number for the library.

	Changes to this number might introduce new features, but they should be
	compatible with existing code.

	For example, it should be safe to upgrade from Panini 1.1.x to 1.2.x,
	although you might have to make some changes to your code.

	\ingroup Globals
*/
#define PANINI_VERSION_MINOR 3

/*!
	Patch version number for the library.

	Changes to this number strictly indicate bug fixes and other clean ups that
	are always safe to integrate in your code.

	For example, you can safely upgrade from Panini 1.2.12 to 1.2.25 without
	having to make changes to your code.

	\ingroup Globals
*/
#define PANINI_VERSION_PATCH 0

/*!
	Complete version number for the library, as a string.

	\ingroup Globals
*/
#define PANINI_VERSION_STR "1.3.0"
