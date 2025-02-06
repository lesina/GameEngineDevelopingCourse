#pragma once

#include <Level.h>
#include <Filesystem.h>

namespace GameEngine::Editor
{
	class LevelSerializer final
	{
	public:
		LevelSerializer() = delete;
		~LevelSerializer() = delete;

	public:
		static void Serialize(const std::string& filepath, const World::Level& level);
		[[nodiscard]] static World::Level Deserialize(const std::string& filepath);
	};
}