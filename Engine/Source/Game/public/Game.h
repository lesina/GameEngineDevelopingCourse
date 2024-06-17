#pragma once

#include <EntityManager.h>
#include <GameFramework/GameFramework.h>
#include <RenderEngine.h>
#include <RenderThread.h>
#include <ScriptManager.h>
#include <Timer.h>
#include <Window/IWindow.h>

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
		void Update(float dt);

	private:
		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;

	private:
		EntitySystem::EntityManager::Ptr m_EntityManager = nullptr;
		ScriptSystem::ScriptManager::Ptr m_ScriptManager = nullptr;
		Render::RenderThread::Ptr m_renderThread = nullptr; 
		GameFramework::Ptr m_GameFramework = nullptr;

		Core::Timer m_GameTimer;
	};
}