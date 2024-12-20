#include <Camera.h>
#include <DefaultGeometry.h>
#include <EntityManager.h>
#include <Game.h>
#include <Input/InputHandler.h>
#include <AudioEngine.h>

namespace GameEngine
{
	Game::Game(std::function<bool()> PlatformLoopFunc) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 12.0f, -10.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 12.0f).Normalized());

		AudioSystem::g_AudioManager = new AudioSystem::AudioManager();

		m_renderThread = std::make_unique<Render::RenderThread>();

		m_EntityManager = std::make_unique<EntitySystem::EntityManager>(m_renderThread.get());
		m_GameFramework = std::make_unique<GameFramework>(m_EntityManager->GetWorld());
		m_ScriptManager = std::make_unique<ScriptSystem::ScriptManager>(m_EntityManager->GetWorld());
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();

			Update(dt);

			m_renderThread->OnEndFrame();

			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		m_EntityManager->Update(dt);
		m_ScriptManager->Update(dt);
		m_GameFramework->Update(dt);
	}
}