#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API FileSystem final
		{
		public:
			struct Deleter {
				void operator()(FileSystem* cmd) { delete cmd; }
			};

			using Ptr = std::unique_ptr<FileSystem, Deleter>;
			using Path = std::filesystem::path;

		private:
			FileSystem() = delete;
			FileSystem(const std::string& root);
			~FileSystem() = default;

		public:
			static FileSystem::Ptr Create(const std::string& root);

			inline Path GetFilePath(const std::string& path) const;
			inline Path GetShaderPath(const std::string& path) const;
			inline Path GetConfigPath(const std::string& path) const;
			inline Path GetScriptsPath(const std::string& path) const;

		private:
			Path m_root;
			Path m_shadersRoot;
			Path m_configsRoot;
			Path m_scriptsRoot;
		};

		extern CORE_API FileSystem::Ptr g_FileSystem;
	}
}