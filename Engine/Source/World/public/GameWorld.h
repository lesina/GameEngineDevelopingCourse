#pragma once

#include <flecs.h>
#include <World/export.h>

#include <Level.h>

namespace GameEngine::World
{
	class WORLD_API GameWorld final
	{
	private:
		GameWorld();
		~GameWorld() = default;

	public:
		static GameWorld* GetInstance();

	public:
		void LoadLevel(flecs::world& flecs, const std::string& levelPath);
		const Level& GetCurrentLevel() const { assert(m_CurrentLevel.has_value()); return m_CurrentLevel.value(); }

	private:
		void LoadCurrentLevel();
		void UnloadCurrentLevel();

	private:
		flecs::world m_World;

		std::optional<Level> m_CurrentLevel = std::nullopt;

		static GameWorld* m_Instance;
	};
}