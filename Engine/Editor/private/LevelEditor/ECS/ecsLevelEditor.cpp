#include <LevelEditor/ECS/ecsLevelEditor.h>
#include<array>

namespace
{
	void ParsePosition(
		const cqe::EntitySystem::LevelEditorECS::PositionDesc& positionDesc,
		cqe::EntitySystem::EditorECS::Position& position
	)
	{
		assert(positionDesc.value);
		assert(positionDesc.value->size() == 3);

		position.x = (*positionDesc.value)[0];
		position.y = (*positionDesc.value)[1];
		position.z = (*positionDesc.value)[2];
	}
}

namespace cqe::EntitySystem::LevelEditorECS
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