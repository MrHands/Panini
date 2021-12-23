// generate a visual studio project file for compiling .ink files to .json

#include <Panini.hpp>

// custom command for outputting xml element

class XmlElement
	: public panini::CommandBase
{

public:
	XmlElement(std::string&& name, std::string&& properties, std::function<void(panini::WriterBase&)>&& callback)
		: m_name(name)
		, m_properties(properties)
		, m_callback(callback)
	{
	}

	virtual void Visit(panini::WriterBase& writer)
	{
		using namespace panini;

		writer << "<" << m_name;

		if (!m_properties.empty())
		{
			writer << " " << m_properties;
		}

		writer << ">" << IndentPush() << NextLine();

		m_callback(writer);

		writer << IndentPop() << "</" << m_name << ">";

		// protip: never end a command with a NextLine() command!
		// because panini is write-forward only, you cannot undo a new line
		// if it turns out you don't need it on the next command
	}

private:
	std::string m_name;
	std::string m_properties;
	std::function<void(panini::WriterBase&)> m_callback;

};

int main(int argc, char** argv)
{
	using namespace panini;

	// logging

	ConsoleWriter logger;

	// use windows-style newlines on the output

	Config config;
	config.chunkIndent = "  ";
	config.chunkNewLine = "\r\n";

	// the CompareWriter will compare the output against an existing file
	// if the generated output was changed it will write it to the specified
	// path

	logger << "Generating MyStory.vcxproj..." << NextLine();

	CompareWriter writer("MyStory.vcxproj", config);

	// the R"XML()XML" syntax is used here to allow us to write strings
	// directly without having to escape characters like "

	writer << R"XML(<?xml version="1.0" encoding="utf-8"?>)XML" << NextLine();
	writer << XmlElement("Project", R"XML(DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003")XML", [](WriterBase& writer) {
		writer << XmlElement("ItemGroup", R"XML(Label="ProjectConfigurations")XML", [](WriterBase& writer) {
			writer << XmlElement("ProjectConfiguration", R"XML(Include="Debug|x64")XML", [](WriterBase& writer) {
				writer << R"XML(<Configuration>Debug</Configuration>)XML" << NextLine();
				writer << R"XML(<Platform>x64</Platform>)XML" << NextLine();
			}) << NextLine();
		}) << NextLine();
		writer << XmlElement("PropertyGroup", R"XML(Label="Globals")XML", [](WriterBase& writer) {
			writer << R"XML(<VCProjectVersion>15.0</VCProjectVersion>)XML" << NextLine();
			writer << R"XML(<WindowsTargetPlatformVersion>10.0.17763.0</WindowsTargetPlatformVersion>)XML" << NextLine();
			writer << R"XML(<ProjectGuid>{C36AB1D3-74C7-4352-B2DB-C7728CFD2297}</ProjectGuid>)XML" << NextLine();
			writer << R"XML(<InklecateDir>$(SolutionDir)dependencies\inklecate-1.0.0\</InklecateDir>)XML" << NextLine();
			writer << R"XML(<NarrativeDir>$(SolutionDir)build\data\stories\</NarrativeDir>)XML" << NextLine();
		}) << NextLine();
		writer << XmlElement("CustomBuild", R"XML(Include="MyStory.ink")XML", [](WriterBase& writer) {
			writer << R"XML(<Message>Building %(Filename).ink...</Message>)XML" << NextLine();
			writer << R"XML(<AdditionalInputs>%(FullPath);$(InklecateDir)\inklecate.exe;%(AdditionalInputs)</AdditionalInputs>)XML" << NextLine();
			writer << R"XML(<Outputs>$(NarrativeDir)%(Filename).json;%(Outputs)</Outputs>)XML" << NextLine();
			writer << R"XML(<Command>"$(InklecateDir)\inklecate.exe" -o "$(NarrativeDir)%(Filename).json" "%(FullPath)"</Command>)XML" << NextLine();
			writer << R"XML(<SubType>Designer</SubType>)XML" << NextLine();
		}) << NextLine();
	});

	logger << "Done." << NextLine();

	return 0;
}
