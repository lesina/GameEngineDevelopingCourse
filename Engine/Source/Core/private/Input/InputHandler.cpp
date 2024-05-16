#include <Camera.h>
#include <Constants.h>
#include <Debug/Console.h>
#include <INIReader.h>
#include <Input/InputHandler.h>

namespace GameEngine::Core
{
	InputHandler::Ptr g_InputHandler = nullptr;

	InputHandler::InputHandler(FileSystem::Path&& config)
		: m_Config(std::move(config))
	{
		INIReader reader(m_Config.string());
		assert(reader.ParseError() >= 0);

		std::set<std::string> fields = reader.GetFields("Keyboard");

		for (const std::string& field : fields)
		{
			std::string eventName = reader.Get("Keyboard", field, "");

			assert(!m_KeyboardEventMap.contains(StringToKeyboardButton(eventName)));
			m_KeyboardEventMap.emplace(StringToKeyboardButton(eventName), field);
		}

		fields = reader.GetFields("Mouse");

		for (const std::string& field : fields)
		{
			std::string eventName = reader.Get("Mouse", field, "");

			assert(!m_MouseEventMap.contains(StringToMouseButton(eventName)));
			m_MouseEventMap.emplace(StringToMouseButton(eventName), field);
		}

		m_ActionMapEventManager = std::make_unique<ActionMapEventManager>();
	}

	void InputHandler::Update()
	{
		for (const KeyboardButton& kb : m_PressedKB)
		{
			m_ActionMapEventManager->CallEvent(m_KeyboardEventMap[kb]);
		}
	}

	void InputHandler::KeyPressed(KeyboardButton kb)
	{
		m_PressedKB.push_back(kb);
	}

	void InputHandler::KeyReleased(KeyboardButton kb)
	{
		m_PressedKB.remove(kb);
	}

	void InputHandler::KeyPressed(MouseButton mb)
	{
		m_PressedMB.push_back(mb);
	}

	void InputHandler::KeyReleased(MouseButton mb)
	{
		m_PressedMB.remove(mb);
	}

	void InputHandler::RegisterCallback(std::string eventName, ActionMapEventManager::Event callback)
	{
		m_ActionMapEventManager->RegisterCallback(eventName, callback);
	}

	InputHandler::Ptr InputHandler::CreateInputHandler(FileSystem::Path&& config)
	{
		return InputHandler::Ptr(new InputHandler(std::move(config)), Deleter());
	}

	void InputHandler::OnMouseMove(float dx, float dy)
	{
		dx *= 0.25 * Math::Constants::PI / 180.f;
		dy *= 0.25 * Math::Constants::PI / 180.f;

		g_MainCamera->Rotate(dx, dy);
	}
}