Panini is a header-only library for generating C++ code written in C++17.

Panini uses a write-only approach to code generation, which means output is only ever appended and never rolled back. This makes Panini very fast in run-time execution because it does not need to track many state variables. The library automatically handles whitespace in your generated code with commands for adding a new line and pushing and popping the indentation level.

Unlike most code generation solutions for C++, Panini does not use a scripting language, as its "scripts" are written in C++. This allows you to create very small executables for code generation that you can step through with a debugger while your code is being output. You can also easily integrate Panini with your existing C++ code and use the same tools for retrieving and processing data as the rest of your codebase.

While Panini is best suited for generating C++, it can be used to generate code for any language, including JavaScript, TypeScript, XML, or even Python. Panini uses the C++ Standard Template Library (STL) and has no other dependencies.

Panini is validated with 100+ unit and integration tests.

# Example

```cpp
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
```

Which will output the following to the console:

```cpp
#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;

	return 0;
}
```

# Installation

Panini comes with a `CMakeLists.txt` configuration file. Although you should refer to the [CMake documentation](https://cmake.org/) for your specific configuration, as an example, you can use the following command to generate a solution for Visual Studio 2017 in the `build` directory:

	cmake -G "Visual Studio 15 2017 Win64" -S . -B build

# Philosophy

Panini is designed to output code with as little processing as possible on the input. The library is designed to be _write-only_, which means you cannot undo already written output. This allows for very fast execution because there is only minimal state to track while running the program.

Panini does not do any string processing itself, but we can recommend the [fmt](https://github.com/fmtlib/fmt) library.

| Type          | Description                                                     |
| ------------- | --------------------------------------------------------------- |
| Chunk         | Strings of characters after input processing                    |
| Command       | Modifies the state of chunks and adds them to the active writer |
| Writer        | Writes chunks to the output and processes commands              |
| WriterConfig  | Output settings for the writer                                  |

## Writing custom commands

Commands range from the humble `NextLine`, which adds a new line chunk, to the more involved `Braces`, which adds opening and closing braces to the output based on the brace breaking style.

Create your own commands by inheriting from the `panini::Command` class and implementing its pure virtual methods. No other registration is required. Because output cannot be undone after it is written, we **strongly advise** not to end your commands with a `NextLine()` instruction.

## Configuring writers

Code generation is always started by declaring an instance of `panini::ConfiguredWriter` and optionally giving it a `panini::WriterConfig` specific to that writer. Panini includes several common writer implementations out of the box. The configuration instance allows you to change the indentation style (tabs or spaces), new line style (Unix or Windows), where opening and closing braces are placed, and other properties that may be specific to your writer. 

```cpp
FileWriterConfig config;
config.indentChunk = "  ";
FileWriter writer("taxes.csv", config);
```

Create your own writer by inheriting from `panini::ConfiguredWriter` and implementing its pure virtual methods. No other registration is required. If your writer has custom properties that should be configured, create a new struct that inherits from `panini::WriterConfig`. See `panini::FileWriterConfig` for an example.
