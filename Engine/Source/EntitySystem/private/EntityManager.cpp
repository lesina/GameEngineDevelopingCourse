#include <EntityManager.h>
#include <ECS/ecsSystems.h>

namespace GameEngine::EntitySystem
{
	EntityManager::EntityManager(Render::RenderThread* const renderThread)
	{
		m_World.set(ECS::RenderThreadPtr{ renderThread });

		ECS::RegisterEcsCoreSystems(m_World);
	}

	void EntityManager::Update(float dt)
	{
		m_World.progress(dt);
	}
}
