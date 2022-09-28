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

#include <filesystem>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <iostream>

#include <Panini.hpp>

struct IniSetting
{
	std::string name;
	std::string value;
};

struct IniSection
{
	std::string name;
	std::vector<IniSetting> settings;
};

 std::optional<std::vector<std::shared_ptr<IniSection>>> ParseIni(const std::filesystem::path& iniPath)
{
	std::ifstream stream(iniPath.string());
	if (!stream.is_open())
	{
		std::cerr << "Failed to open \"" << iniPath.string() << "\" for reading." << std::endl;

		return {};
	}

	std::vector<std::shared_ptr<IniSection>> allSections;
	std::shared_ptr<IniSection> section;

	std::regex headerSearch(R"(\[(\w+)\])");
	std::regex settingSearch(R"((\w+) = (.+))");

	std::string line;
	while (std::getline(stream, line))
	{
		// header

		std::smatch headerMatch;
		if (std::regex_search(line, headerMatch, headerSearch))
		{
			section = allSections.emplace_back(new IniSection);
			section->name = headerMatch[1];
		}

		// setting

		std::smatch settingMatch;
		if (std::regex_search(line, settingMatch, settingSearch))
		{
			IniSetting& setting = section->settings.emplace_back();
			setting.name = settingMatch[1];
			setting.value = settingMatch[2];
		}
	}

	return allSections;
}

int main(int argc, char** argv)
{
	using namespace panini;

	auto result = ParseIni(std::filesystem::current_path() / "Hierarchy.ini");
	if (!result.has_value())
	{
		return -1;
	}

	std::vector<std::shared_ptr<IniSection>> sections = std::move(result.value());

	return 0;
}
