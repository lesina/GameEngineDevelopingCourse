#include <CameraManager.h>
#include <DefaultGeometry.h>
#include <EntityManager.h>
#include <Game.h>
#include <GUIContext.h>
#include <Input/InputHandler.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_CameraManager = std::make_unique<Core::CameraManager>();

		m_renderThread = std::make_unique<Render::RenderThread>();

		GUI::GUIContext::GetInstance()->PlatformInit();

		m_EntityManager = std::make_unique<EntitySystem::EntityManager>(m_renderThread.get());
		m_GameFramework = std::make_unique<GameFramework>(m_EntityManager->GetWorld());
		m_ScriptManager = std::make_unique<ScriptSystem::ScriptManager>(m_EntityManager->GetWorld());
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		m_renderThread->WaitForRenderEngineToInit();

		Core::g_GUIWindowsCallback = [](Core::PackedVariables& packedVariables) { return GUI::GUIContext::GetInstance()->UpdateInput(packedVariables); };

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		m_EntityManager->Update(dt);
		m_ScriptManager->Update(dt);
		m_GameFramework->Update(dt);

		// Should be updated last since a lot of logic is binned to the input handler
		Core::InputHandler::GetInstance()->Update(dt);
	}
}