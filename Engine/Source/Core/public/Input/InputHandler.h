#pragma once

#include <Core/export.h>
#include <FileSystem.h>
#include <Input/Buttons.h>
#include <Input/ActionMapEventManager.h>
#include <Vector.h>

namespace GameEngine::Core
{
	class CORE_API InputHandler final
	{
	public:
		struct Deleter {
			void operator()(InputHandler* cmd) { delete cmd; }
		};

		using Ptr = std::unique_ptr<InputHandler, Deleter>;

	private:
		InputHandler() = delete;
		InputHandler(FileSystem::Path&& config);
		~InputHandler() = default;

	public:
		static InputHandler::Ptr CreateInputHandler(FileSystem::Path&& config);

		void Update();
		void KeyPressed(KeyboardButton kb);
		void KeyReleased(KeyboardButton kb);
		void KeyPressed(MouseButton kb);
		void KeyReleased(MouseButton mb);
		void OnMouseMove(float dx, float dy);

		void RegisterCallback(std::string eventName, ActionMapEventManager::Event callback);

	private:
		std::unordered_map<KeyboardButton, std::string> m_KeyboardEventMap;
		std::unordered_map<MouseButton, std::string> m_MouseEventMap;

		std::list<KeyboardButton> m_PressedKB;
		std::list<MouseButton> m_PressedMB;

		FileSystem::Path m_Config;

		ActionMapEventManager::Ptr m_ActionMapEventManager = nullptr;
	};

	extern CORE_API InputHandler::Ptr g_InputHandler;
}