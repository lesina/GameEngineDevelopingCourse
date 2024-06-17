#pragma once

#include <EntitySystem/export.h>
#include <flecs.h>
#include <RenderThread.h>
#include <Input/InputHandler.h>

namespace GameEngine
{
	namespace EntitySystem
	{
		class ENTITY_SYSTEM_API EntityManager final
		{
		public:
			using Ptr = std::unique_ptr<EntityManager>;

		public:
			EntityManager() = delete;
			EntityManager(Render::RenderThread* const renderThread);
			~EntityManager() = default;

		public:
			void Update(float dt);

			flecs::world& GetWorld() { return m_World; }

		private:
			flecs::world m_World;
		};
	}
}
