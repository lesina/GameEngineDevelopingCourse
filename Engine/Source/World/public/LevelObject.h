#pragma once

#include <World/export.h>

namespace GameEngine::World
{
	class WORLD_API LevelObject final
	{
	public:
		using ComponentName = std::string;
		using ComponentDesc = std::string;
		using Component = std::pair<ComponentName, ComponentDesc>;
		using ComponentList = std::vector<Component>;

	public:
		LevelObject() = default;
		~LevelObject() = default;

	public:
		void SetName(const char* name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }
		void AddComponent(const ComponentName& name, const ComponentDesc& desc);
		const ComponentList& GetComponents() const { return m_ComponentList; }

	private:
		std::string m_Name = "None";
		ComponentList m_ComponentList;
	};
}