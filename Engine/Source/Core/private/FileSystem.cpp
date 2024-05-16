#include <FileSystem.h>

namespace GameEngine::Core
{
	FileSystem::Ptr g_FileSystem = nullptr;

	FileSystem::FileSystem(const std::string& root)
		: m_root(root)
	{
		m_root.append("Assets");

		m_shadersRoot = m_root / "Shaders";
		m_configsRoot = m_root / "Configs";
	}

	std::wstring FileSystem::GetFilePath(const std::string& path) const
	{
		return m_root/ path;
	}

	std::wstring FileSystem::GetShaderPath(const std::string& path) const
	{
		return m_shadersRoot / path;
	}

	std::wstring FileSystem::GetConfigPath(const std::string& path) const
	{
		return m_configsRoot / path;
	}

	FileSystem::Ptr FileSystem::Create(const std::string& root)
	{
		return FileSystem::Ptr(new FileSystem(root), Deleter());
	}
}