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
