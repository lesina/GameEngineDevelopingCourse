#include <Game.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		m_renderEngine = std::make_unique<Render::RenderEngine>();
		
		ProcessSystemParams();
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		bool quit = false;
		while (!quit)
		{
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

            ProcessSystemParams();

			m_renderEngine->Update();
		}
	}

	void Game::ProcessSystemParams()
	{
		if (m_renderEngine->GetSwapChainWidth() != Core::MainWindowsApplication->GetWidth() ||
			m_renderEngine->GetSwapChainHeight() != Core::MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			m_renderEngine->OnResize(Core::MainWindowsApplication->GetWidth(), Core::MainWindowsApplication->GetHeight());
		}
	}
}