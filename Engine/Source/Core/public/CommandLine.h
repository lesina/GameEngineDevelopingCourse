#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API CommandLine final
		{
		public:
			struct Deleter {
				void operator()(CommandLine* cmd) { delete cmd; }
			};

			using Ptr = std::unique_ptr<CommandLine, Deleter>;

		private:
			CommandLine() = delete;
			CommandLine(const std::vector<std::string>& commandLine);
			~CommandLine() = default;

		public:
			static CommandLine::Ptr Parse(const std::vector<std::wstring>& commandLine);
			static CommandLine::Ptr Parse(const std::vector<std::string>& commandLine);
			const std::string& GetAttribute(const std::string& key) { return m_arguments[key]; }
			bool HasAttribute(const std::string& key) { return m_arguments.contains(key); }

		private:
			std::unordered_map<std::string, std::string> m_arguments;
		};

		extern CORE_API CommandLine::Ptr g_CommandLineArguments;
	}
}