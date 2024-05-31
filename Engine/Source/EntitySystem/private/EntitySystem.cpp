#include <EntitySystem.h>
#include <ECS/ecsSystems.h>

namespace GameEngine::EntitySystem
{
	EntitySystem::EntitySystem(Render::RenderThread* const renderThread)
	{
		m_World.set(ECS::RenderThreadPtr{ renderThread });
	}

	void EntitySystem::Update(float dt)
	{
		m_World.progress(dt);
	}
}
