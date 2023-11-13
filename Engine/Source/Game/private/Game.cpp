#include <Game.h>
#include <RenderEngine.h>

namespace GameEngine
{
	Game::Game(Core::IWindow* window) :
		m_window(std::move(window))
	{

	}

	void Game::Run()
	{
		std::unique_ptr<Render::RenderEngine> renderEngine = std::make_unique<Render::RenderEngine>();

		while (true)
		{
			renderEngine->Update();
		}
	}
}