#include <CommandLine.h>
#include <Debug/Console.h>

namespace GameEngine::Core
{
	CommandLine::Ptr g_CommandLineArguments = nullptr;

	CommandLine::CommandLine(const std::vector<std::string>& commandLine)
	{
		for (const std::string_view& commandLineArg : commandLine)
		{
			size_t assignOp = commandLineArg.find("=");

			bool commandLineArgIsValid =
				assignOp != std::string_view::npos ?
				(assignOp > 1 && assignOp < commandLineArg.size() - 1) :
				true;

			if (!commandLineArg.starts_with("-") && !commandLineArgIsValid) [[unlikely]]
			{
				Console::PrintDebug(std::format("{} is not a valid command line arg format", commandLineArg));
				continue;
			}

			size_t valueStart = assignOp + 1;
			size_t valueEnd = commandLineArg.size() - 1;

			if (assignOp != std::string_view::npos)
			{
				if (commandLineArg[valueStart] == '\"' && commandLineArg[valueEnd] == '\"')
				{
					++valueStart;
					--valueEnd;
				}
			}

			std::string key(commandLineArg.substr(1, assignOp - 1));
			std::string value(
				assignOp != std::string_view::npos ?
				commandLineArg.substr(valueStart, valueEnd - valueStart + 1) :
				""
			);

			m_arguments.emplace(key, value);
		}
	}

	CommandLine::Ptr CommandLine::Parse(const std::vector<std::wstring>& wCommandLine)
	{
		std::vector<std::string> commandLine;

		for (const std::wstring& wCommandLineArg : wCommandLine)
		{
			std::string commandLineArg;
			std::transform(wCommandLineArg.begin(), wCommandLineArg.end(), std::back_inserter(commandLineArg), [](wchar_t c) {
				return (char)c;
			});
			
			commandLine.push_back(commandLineArg.c_str());
		}

		return Parse(commandLine);
	}

	CommandLine::Ptr CommandLine::Parse(const std::vector<std::string>& commandLine)
	{
		return CommandLine::Ptr(new CommandLine(commandLine), Deleter());
	}
}