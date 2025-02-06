#include <GameEditor.h>

#include <Debug/Console.h>
#include <EditorECS/ecsEditor.h>
#include <Camera.h>
#include <DefaultGeometry.h>
#include <Window/IWindow.h>
#include <GUIContext.h>
#include <RenderEngine.h>
#include <LevelEditor/LevelEditor.h>

#include <imgui.h>

namespace GameEngine
{
	GameEditor::GameEditor(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 12.0f, -10.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 12.0f));

		GUI::GUIContext::GetInstance()->PlatformInit();
		m_renderThread = std::make_unique<Render::RenderThread>();

		m_EntityManager = std::make_unique<EntitySystem::EntityManager>(m_renderThread.get());

		// flecs::world should be initialized first before getting the reference to actual world within the compilable unit
		flecs::world world; world = m_EntityManager->GetWorld().get_world();
		// huge refactoring requires or flecs custom fix because this api redundancy is just annoying

		flecs::entity camera = m_EntityManager->GetWorld().entity()
			.set(EntitySystem::EditorECS::Position{ 0.0f, 12.0f, -10.0f })
			.set(EntitySystem::EditorECS::Speed{ 10.f })
			.set(EntitySystem::EditorECS::CameraPtr{ Core::g_MainCamera });

		EntitySystem::EditorECS::RegisterEditorEcsControlSystems(m_EntityManager->GetWorld());

		m_UIWindows.reserve(32);

		m_UIWindows.push_back(new Editor::LevelEditor(m_EntityManager->GetWorld()));
	}

	void GameEditor::Run()
	{
		assert(PlatformLoop != nullptr);

		m_renderThread->WaitForRenderEngineToInit();

		ImGui::SetCurrentContext(GUI::GUIContext::GetInstance()->GetImGuiContext());

		Core::g_GUIWindowsCallback = [](Core::PackedVariables& packedVariables) { return GUI::GUIContext::GetInstance()->UpdateInput(packedVariables); };

		bool quit = false;
		while (!quit)
		{
			m_Timer.Tick();
			float dt = m_Timer.GetDeltaTime();

			GUI::GUIContext::GetInstance()->NewFrame();

			Core::g_MainWindowsApplication->Update();

			Update(dt);

			GUI::GUIContext::GetInstance()->Draw();

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void GameEditor::Update(float dt)
	{
		for (GUI::UIWindow* guiWindow : m_UIWindows)
		{
			guiWindow->Update(dt);
			guiWindow->Draw();
		}

		m_EntityManager->Update(dt);

		// We want the camera to move only when we are clicking RMB outside the interface
		if (!GUI::GUIContext::GetInstance()->IsUIHovered() && !Core::g_MainWindowsApplication->IsMouseCaptured() && Core::InputHandler::GetInstance()->IsKeyPressed(Core::MouseButton::RMB) && Core::g_MainWindowsApplication->IsFocused())
		{
			Core::g_MainWindowsApplication->CaptureMouse();
			GUI::GUIContext::GetInstance()->UnFocus();
		}
		else if (!Core::InputHandler::GetInstance()->IsKeyPressed(Core::MouseButton::RMB) && Core::g_MainWindowsApplication->IsMouseCaptured())
		{
			Core::g_MainWindowsApplication->ReleaseMouse();
		}

		// Should be updated last since a lot of logic is binned to the input handler
		Core::InputHandler::GetInstance()->Update(dt);
	}
}