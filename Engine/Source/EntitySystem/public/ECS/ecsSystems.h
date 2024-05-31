#pragma once

namespace GameEngine
{
	namespace Render
	{
		class RenderThread;
	}

	namespace EntitySystem::ECS
	{
		struct RenderThreadPtr
		{
			Render::RenderThread* ptr;
		};
	}
}
