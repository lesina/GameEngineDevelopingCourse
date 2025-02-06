#include <LevelEditor/ECS/ecsLevelEditor.h>

namespace
{
	void ParsePosition(
		const GameEngine::EntitySystem::LevelEditorECS::PositionDesc& positionDesc,
		GameEngine::EntitySystem::EditorECS::Position& position
	)
	{
		assert(positionDesc.value);
		assert(std::ranges::count(*positionDesc.value, ',') == 2);

		const char* compValue = positionDesc.value->c_str();
		char* end;

		float f = std::strtof(compValue, &end);
		position.x = f;
		compValue = end + 1;

		f = std::strtof(compValue, &end);
		position.y = f;
		compValue = end + 1;

		f = std::strtof(compValue, &end);
		position.z = f;
	}
}

namespace GameEngine::EntitySystem::LevelEditorECS
{
	void RegisterLevelEditorEcsSystems(flecs::world& world)
	{
		world.system<const PositionDesc, EntitySystem::EditorECS::Position>()
			.each([&](const PositionDesc& positionDesc, EntitySystem::EditorECS::Position& position)
				{
					ParsePosition(positionDesc, position);
				});
	}
}