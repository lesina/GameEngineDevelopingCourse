#include <LevelEditor/ECS/ecsLevelEditor.h>


namespace GameEngine {
	Math::Vector3f ParseStringToFloat3(const World::LevelObject::ComponentDesc& componentDesc) {

		assert(std::ranges::count(componentDesc, ',') == 2);

		Math::Vector3f vec = Math::Vector3f::Zero();

		const char* compValue = componentDesc.c_str();
		char* end;

		float f = std::strtof(compValue, &end);
		vec.x = f;
		compValue = end + 1;

		f = std::strtof(compValue, &end);
		vec.y = f;
		compValue = end + 1;

		f = std::strtof(compValue, &end);
		vec.z = f;

		return vec;
	}
}

namespace
{
	void ParsePosition(
		const GameEngine::EntitySystem::LevelEditorECS::PositionDesc& positionDesc,
		GameEngine::EntitySystem::EditorECS::Position& position
	)
	{
		assert(positionDesc.value);
		GameEngine::Math::Vector3f vecPos = GameEngine::ParseStringToFloat3(*positionDesc.value);
		position.x = vecPos.x;
		position.y = vecPos.y;
		position.z = vecPos.z;
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