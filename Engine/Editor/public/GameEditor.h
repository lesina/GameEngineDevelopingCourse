#pragma once

#include <EntityManager.h>
#include <Input/InputHandler.h>
#include <RenderThread.h>
#include <Timer.h>

namespace GameEngine
{
	namespace GUI
	{
		class UIWindow;
	}

	class GameEditor final
	{
	public:
		GameEditor() = delete;
		GameEditor(
			std::function<bool()> PlatformLoopFunc
		);

	public:
		void Run();
		void Update(float dt);

	private:
		Core::Timer m_Timer;
		std::vector<GUI::UIWindow*> m_UIWindows;

	private:
		Render::RenderThread::Ptr m_renderThread = nullptr;
		EntitySystem::EntityManager::Ptr m_EntityManager = nullptr;

		// The main idea behind having this functor is to abstract the common code from the platfrom-specific code
		std::function<bool()> PlatformLoop = nullptr;
	};
}