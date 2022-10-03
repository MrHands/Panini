Panini is a header-only library for generating C++ code written in C++17.

Panini allows you to write small executables for code-generation that you can step through with a debugger while your code is being output. Because the code generation is itself written in C++, you can call the same functions and use the same tools as the rest of your codebase.

The library takes a write-only approach to code generation, which means output is only ever appended to, and never rolled back. This means Panini can be very fast because the only state it has to keep track of is whether a new line was just started.

While Panini is best suited for generating C++, you can use it to generate code for any programming language, including JavaScript, TypeScript, XML, or even Python. The library uses the C++ Standard Template Library (STL) and has no other dependencies.

# Example

	#include <Panini.hpp>

	int main(int argc, char** argv)
	{
		using namespace panini;

		ConsoleWriter writer;

		writer << Include("iostream", IncludeStyle::AngularBrackets) << NextLine();

		writer << NextLine();

		writer << Scope("int main(int argc, char** argv)", [](WriterBase& writer)
		{
			writer << R"(std::cout << "Hello, World!" << std::endl;)" << NextLine();

			writer << NextLine();

			writer << "return 0;" << NextLine();
		}) << NextLine();

		return 0;
	}

Which will output the following to the console:

	#include <iostream>

	int main(int argc, char** argv)
	{
		std::cout << "Hello, World!" << std::endl;

		return 0;
	}

# Installation

Panini comes with a `CMakeLists.txt` configuration file. Although you should refer to the [CMake documentation](https://cmake.org/) for your specific configuration, as an example, you can use the following command to generate a solution for Visual Studio 2017 in the `build` directory:

	cmake -G "Visual Studio 15 2017 Win64" -S . -B build

# Philosophy

Panini is designed to output code with as little processing as possible on the input. The library is designed to be _write-forward only_, which means you cannot undo output that was already written. This allows for very fast execution because there is only minimal state to keep track of while running through the program.

| Type    | Description                                         |
| ------- | --------------------------------------------------- |
| Chunk   | Strings of characters after input processing        |
| Command | Modifies the state of and adds chunks to the writer |
| Writer  | Writes chunks to the output and processes commands  |
| Config  | Global configuration for the writer                 |

## Chunks

Panini does not do any string processing itself but we can recommend the [fmt](https://github.com/fmtlib/fmt) library for doing so.

## Commands

Commands can range from the humble `NextLine`, which adds a new line chunk, to the more involved `Braces`, which adds opening and closing braces to the output based on the brace breaking style.

You can create a custom command by inheriting from `panini::CommandBase` and implementing the `Visit` method.

## Writers

You always start your code generation by declaring a `Writer` and optionally giving it a `Config`. Panini comes with several writer implementations by default.

	Config config;
	config.indentChunk = "  ";
	FileWriter writer("taxes.csv", config);

You can create a custom writer by inheriting from `panini::WriterBase` and implementing the `Write` method.

## Configuration

Config allows you to change the indentation style (tabs or spaces), new line style (Unix or Windows), and brace breaking style for the writer.
