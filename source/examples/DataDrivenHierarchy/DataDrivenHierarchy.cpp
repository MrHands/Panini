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

// adds a component to our generated game objects

class AddComponentCommand
	: public panini::Command
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

// generates a game object definition from an ini section

class GameObjectCommand
	: public panini::Command
{

public:
	GameObjectCommand(const IniSection& section)
		: m_section(section)
	{
		// resolve inheritance

		auto inheritsSetting = GetSetting("inherits");
		if (inheritsSetting.has_value())
		{
			m_isPure = false;

			auto parentFound = std::find_if(g_IniSections.begin(), g_IniSections.end(), [&inheritsSetting](const std::shared_ptr<IniSection>& it) {
				return it->name == inheritsSetting->value;
			});
			if (parentFound != g_IniSections.end())
			{
				auto parentCopy = (*parentFound)->settings;
				m_section.settings.merge(parentCopy);
			}
		}
	}

	bool GetIsPure() const
	{
		return m_isPure;
	}

	virtual void Visit(panini::WriterBase& writer) final
	{
		using namespace panini;

		std::stringstream functionScope;
		functionScope << "GameObject* Create" << m_section.name << "()";

		writer << Scope(functionScope.str(), [this](WriterBase& writer) {
			writer << "GameObject* gameObject = new GameObject();" << NextLine();

			// transform

			if (GetSetting("add_transform").has_value())
			{
				writer << AddComponentCommand("TransformComponent", {}) << NextLine();
			}

			// sprite

			if (GetSetting("add_sprite").has_value())
			{
				writer << AddComponentCommand("SpriteComponent", {
					std::string("\"") + GetSetting("sprite")->value + "\""
				}) << NextLine();
			}

			// life

			if (GetSetting("add_life").has_value())
			{
				std::vector<std::string> parameters = {
					GetSetting("health")->value
				};
				if (GetSetting("bonus").has_value())
				{
					parameters.push_back(GetSetting("bonus")->value);
				}

				writer << AddComponentCommand("LifeComponent", parameters) << NextLine();
			}

			// player behavior

			if (GetSetting("add_player").has_value())
			{
				writer << AddComponentCommand("PlayerBehavior", {}) << NextLine();
			}

			// enemy behavior

			if (GetSetting("add_enemy").has_value())
			{
				writer << AddComponentCommand("EnemyBehavior", {}) << NextLine();
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

	bool m_isPure = true;

};

int main(int argc, char** argv)
{
	(int)argc;
	(int)argv;

	using namespace panini;

	auto result = ParseIni(std::filesystem::current_path() / "Hierarchy.ini");
	if (!result.has_value())
	{
		return -1;
	}

	g_IniSections = std::move(result.value());

	// The DebugWriter will output to the console and stop execution at the end
	// of each line. This allows you to step through the code generation line-
	// by-line in order to find out where it may go wrong.

	for (auto section : g_IniSections)
	{
		GameObjectCommand gameObject(*section);
		if (gameObject.GetIsPure())
		{
			// "pure" game objects cannot be instantiated

			continue;
		}

		DebugWriter writer;
		writer << std::move(gameObject);
		writer.Commit();
	}

	return 0;
}
