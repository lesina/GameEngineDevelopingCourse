#pragma once

#include <World/export.h>

#include <LevelObject.h>

namespace GameEngine::World
{
	class WORLD_API Level final
	{
	public:
		using LevelObjectList = std::vector<LevelObject>;

	public:
		Level() = delete;
		Level(std::string& name);
		~Level() = default;

	public:
		void AddLevelObject(const LevelObject& levelObject);
		const LevelObjectList& GetLevelObjects() const { return m_LevelObjectList; }

	private:
		std::string m_Name;
		LevelObjectList m_LevelObjectList;
	};
}