#pragma once

#include <flecs.h>
#include <Geometry.h>

struct GeometryPtr
{
	GameEngine::RenderCore::Geometry* ptr;
};

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

		struct RenderObjectPtr
		{
			GameEngine::Render::RenderObject* ptr = nullptr;

			GameEngine::Render::RenderObject** GetAddressOf()
			{
				return &ptr;
			}
		};

		void RegisterEcsCoreSystems(flecs::world& world);
	}
}
