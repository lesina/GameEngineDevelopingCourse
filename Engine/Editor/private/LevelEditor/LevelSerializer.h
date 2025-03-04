#pragma once

#include <Level.h>
#include <Filesystem.h>

namespace cqe::Editor
{
	class LevelSerializer final
	{
	public:
		LevelSerializer() = delete;
		~LevelSerializer() = delete;

	public:
		static void Serialize(const std::string& filepath, const World::Level& level, const std::unordered_map<std::string, std::array<float, 3>>& objectPositions);
		[[nodiscard]] static World::Level Deserialize(const std::string& filepath);
	};
}