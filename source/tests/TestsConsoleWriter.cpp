#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(ConsoleWriter, Write)
{
	using namespace panini;

	ConsoleWriter w;
	w << "Hello from console!" << NextLine();
	w << IndentPush() << "And me!" << NextLine();
}
