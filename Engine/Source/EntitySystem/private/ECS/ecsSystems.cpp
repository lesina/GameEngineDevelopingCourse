#pragma once

#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>

namespace GameEngine::EntitySystem::ECS
{
	void RegisterEcsCoreSystems(flecs::world& world)
	{
		static const RenderThreadPtr* renderThread = world.get<RenderThreadPtr>();

		world.system<const GeometryPtr, RenderObjectPtr>()
			.each([&](flecs::entity e, const GeometryPtr& geometry, RenderObjectPtr& renderObject)
		{
			renderThread->ptr->EnqueueCommand(Render::ERC::CreateRenderObject, geometry.ptr, renderObject.ptr);
			e.remove<GeometryPtr>();
		});
	}
}
