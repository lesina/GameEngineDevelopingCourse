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
		m_scriptsRoot = m_root / "Scripts";
	}

	FileSystem::Path FileSystem::GetFilePath(const std::string& path) const
	{
		return m_root / path;
	}

	FileSystem::Path FileSystem::GetShaderPath(const std::string& path) const
	{
		return m_shadersRoot / path;
	}

	FileSystem::Path FileSystem::GetConfigPath(const std::string& path) const
	{
		return m_configsRoot / path;
	}

	FileSystem::Path FileSystem::GetScriptsPath(const std::string& path) const
	{
		return m_scriptsRoot / path;
	}

	FileSystem::Ptr FileSystem::Create(const std::string& root)
	{
		return FileSystem::Ptr(new FileSystem(root), Deleter());
	}
}