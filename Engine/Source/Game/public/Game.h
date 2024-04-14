#pragma once

#include <Window/IWindow.h>
#include <RenderEngine.h>

namespace GameEngine
{
	class Game final
	{
	public:
		Game() = delete;
		Game(
			std::function<bool()> PlatformLoopFunc
		);

	public:
		void Run();

		inline void ProcessSystemParams();

	private:
		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;

	private:
		std::unique_ptr<Render::RenderEngine> m_renderEngine;
	};
}