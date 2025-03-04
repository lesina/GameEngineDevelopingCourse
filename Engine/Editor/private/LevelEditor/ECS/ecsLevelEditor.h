#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>

namespace cqe::EntitySystem::LevelEditorECS
{
	struct PositionDesc
	{
		const std::array<float, 3>* value;
	};

	void RegisterLevelEditorEcsSystems(flecs::world& world);
}