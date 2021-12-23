Panini is a header-only library for generating C++ code written in C++17. The main advantage is that this allows you to write small programs for code-generation that you can step through with a debugger while your code is being output.

While Panini is best suited for generating C++, you can use it to generate code for any language, including JavaScript, TypeScript, XML, or even Python. The library has no dependencies except for STL which should make it easy to integrate into your existing codebase.

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

Panini comes with a `CMakeLists.txt` configuration file. As an example, you can use the following command to generate a solution for Visual Studio 2017 in the `build` directory:

	cmake -G "Visual Studio 15 2017 Win64" -S . -B build

