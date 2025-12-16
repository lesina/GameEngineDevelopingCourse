#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>
#include <Vector.h>

namespace GameEngine {
	Math::Vector3f ParseStringToFloat3(const World::LevelObject::ComponentDesc& componentDesc);

	namespace EntitySystem::LevelEditorECS {

		struct PositionDesc
		{
			const World::LevelObject::ComponentDesc* value;
		};

		void RegisterLevelEditorEcsSystems(flecs::world& world);
	}

}