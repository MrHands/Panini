#include <gtest/gtest.h>
#include <Panini.hpp>

TEST(Scope, Regular)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("bool isUserAdmin(const char* username)", [](WriterBase& writer) {
		writer << "return true;" << NextLine();
	});

	EXPECT_STREQ(R"(bool isUserAdmin(const char* username)
{
	return true;
})", t.c_str());
}

TEST(Scope, Empty)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("", [](WriterBase& writer) {
		writer << "bool game_running = true;" << NextLine();
	});

	EXPECT_STREQ(R"({
	bool game_running = true;
})", t.c_str());
}

TEST(Scope, OverrideBracesAttach)
{
	using namespace panini;

	std::string t;
	StringWriter w(t);

	w << Scope("namespace panini", [](WriterBase& writer) {
		writer << "struct Config;" << NextLine();
	}, BraceBreakingStyle::Attach);

	EXPECT_STREQ(R"(namespace panini {
	struct Config;
})", t.c_str());
}

TEST(Scope, ConfigBracesAttach)
{
	using namespace panini;

	Config c;
	c.braces = BraceBreakingStyle::Attach;
	std::string t;
	StringWriter w(t, c);

	w << Scope("struct Public", [](WriterBase& writer) {
		writer << "Public() = default;" << NextLine();
	});

	EXPECT_STREQ(R"(struct Public {
	Public() = default;
})", t.c_str());
}
