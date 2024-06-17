#pragma once

#include <flecs.h>
#include <Geometry.h>

namespace GameEngine
{
	namespace Render
	{
		class RenderThread;
		class RenderObject;
	}

	namespace EntitySystem::ECS
	{
		struct RenderThreadPtr
		{
			Render::RenderThread* ptr;
		};

		struct GeometryPtr
		{
			GameEngine::RenderCore::Geometry::Ptr ptr;
		};

		struct RenderObjectPtr
		{
			GameEngine::Render::RenderObject* ptr = nullptr;
		};

		void RegisterEcsCoreSystems(flecs::world& world);
	}
}
