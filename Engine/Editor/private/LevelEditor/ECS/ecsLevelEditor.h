#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>

namespace GameEngine::EntitySystem::LevelEditorECS
{
	struct PositionDesc
	{
		const World::LevelObject::ComponentDesc* value;
	};

	void RegisterLevelEditorEcsSystems(flecs::world& world);
}