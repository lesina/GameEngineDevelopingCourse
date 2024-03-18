#include <Game.h>

namespace GameEngine
{
	Game::Game(
		Core::Window::Ptr window,
		std::function<bool()> PlatformLoopFunc
	) :
		m_window(window),
		PlatformLoop(PlatformLoopFunc)
	{
		m_renderEngine = std::make_unique<Render::RenderEngine>(window);
		m_renderEngine->OnResize();
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		bool quit = false;
		while (!quit)
		{
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderEngine->Update();
		}
	}
}