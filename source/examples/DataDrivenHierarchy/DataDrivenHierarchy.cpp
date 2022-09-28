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
#include <sstream>
#include <iostream>

#include <Panini.hpp>

// ini parsing

struct IniSetting
{
	std::string name;
	std::string value;
};

struct IniSection
{
	std::string name;
	std::map<std::string, IniSetting> settings;
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
			IniSetting setting;
			setting.name = settingMatch[1];
			setting.value = settingMatch[2];

			section->settings.insert_or_assign(setting.name, setting);
		}
	}

	return allSections;
}

std::vector<std::shared_ptr<IniSection>> g_IniSections;

class AddComponentCommand
	: public panini::CommandBase
{

public:
	AddComponentCommand(const std::string& typeName, const std::vector<std::string>& parameters)
		: m_typeName(typeName)
		, m_parameters(parameters)
	{
	}

	virtual void Visit(panini::WriterBase& writer) final
	{
		using namespace panini;

		writer << "gameObject->AddComponent<" << m_typeName << ">(";
		writer << CommaList(m_parameters.begin(), m_parameters.end());
		writer << ");";
	}

private:
	std::string m_typeName;
	std::vector<std::string> m_parameters;


};

class GameObjectCommand
	: public panini::CommandBase
{

public:
	GameObjectCommand(const IniSection& section)
		: m_section(section)
	{
		// resolve inheritance

		auto inheritsFound = m_section.settings.find("inherits");
		if (inheritsFound != m_section.settings.end())
		{
			auto parentFound = std::find_if(g_IniSections.begin(), g_IniSections.end(), [&inheritsFound](const std::shared_ptr<IniSection>& it) {
				return it->name == inheritsFound->second.value;
			});
			if (parentFound != g_IniSections.end())
			{
				m_section.settings.merge((*parentFound)->settings);
			}
		}
	}

	virtual void Visit(panini::WriterBase& writer) final
	{
		using namespace panini;

		std::stringstream functionScope;
		functionScope << "GameObject* Create" << m_section.name << "()";

		writer << Scope(functionScope.str(), [this](WriterBase& writer) {
			writer << "GameObject* gameObject = new GameObject();" << NextLine();

			std::optional<IniSetting> transformSetting = GetSetting("add_transform");
			if (transformSetting.has_value())
			{
				writer << AddComponentCommand("TransformComponent", {}) << NextLine();
			}

			std::optional<IniSetting> spriteSetting = GetSetting("add_sprite");
			if (spriteSetting.has_value())
			{
				writer << AddComponentCommand("SpriteComponent", {
					std::string("\"") + GetSetting("sprite")->value + "\""
				}) << NextLine();
			}

			writer << "return gameObject;" << NextLine();
		});
	}

private:
	std::optional<IniSetting> GetSetting(const std::string& name)
	{
		auto found = m_section.settings.find(name);
		if (found == m_section.settings.end())
		{
			return {};
		}

		return found->second;
	}

private:
	IniSection m_section;

	std::string m_className;

};

int main(int argc, char** argv)
{
	using namespace panini;

	auto result = ParseIni(std::filesystem::current_path() / "Hierarchy.ini");
	if (!result.has_value())
	{
		return -1;
	}

	g_IniSections = std::move(result.value());

	for (auto section : g_IniSections)
	{
		DebugWriter writer;

		writer << GameObjectCommand(*section);

		writer.Commit();
	}

	return 0;
}
