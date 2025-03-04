#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>

namespace cqe::EntitySystem::LevelEditorECS
{
	struct PositionDesc
	{
		const std::vector<float>* value;
	};

	void RegisterLevelEditorEcsSystems(flecs::world& world);
}