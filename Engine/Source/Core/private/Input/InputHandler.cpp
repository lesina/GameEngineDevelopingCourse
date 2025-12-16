#include <Camera.h>
#include <Constants.h>
#include <INIReader.h>
#include <Input/InputHandler.h>

namespace GameEngine::Core
{
	InputHandler* InputHandler::m_Instance = nullptr;

	InputHandler::InputHandler()
	{
		m_PressedButtons.reset();
	}

	InputHandler* InputHandler::GetInstance()
	{
		if (m_Instance == nullptr) [[unlikely]]
		{
			m_Instance = new InputHandler();
		}

		return m_Instance;
	}

	void InputHandler::KeyPressed(KeyboardButton kb)
	{
		// Delete this logic when all the buttons
		if (kb == KeyboardButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * static_cast<size_t>(kb), true);
	}

	void InputHandler::SetWasKeyPressed(KeyboardButton kb)
	{
		if (kb == KeyboardButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * static_cast<size_t>(kb) + 1, true);
	}

	void InputHandler::KeyReleased(KeyboardButton kb)
	{
		// Delete this logic when all the buttons
		if (kb == KeyboardButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * static_cast<size_t>(kb), false);
		m_PressedButtons.set(2 * static_cast<size_t>(kb) + 1, false);
	}

	void InputHandler::KeyPressed(MouseButton mb)
	{
		// Delete this logic when all the buttons
		if (mb == MouseButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb), true);
	}

	void InputHandler::SetWasKeyPressed(MouseButton mb)
	{
		if (mb == MouseButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb) + 1, true);
	}

	void InputHandler::KeyReleased(MouseButton mb)
	{
		// Delete this logic when all the buttons
		if (mb == MouseButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb), false);
		m_PressedButtons.set(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb) + 1, false);
	}

	bool InputHandler::IsKeyPressed(KeyboardButton kb) const
	{
		assert(kb != KeyboardButton::UNKNOWN);
		return m_PressedButtons.test(2 * static_cast<size_t>(kb));
	}

	bool InputHandler::IsKeyPressed(MouseButton mb) const
	{
		assert(mb != MouseButton::UNKNOWN);
		return m_PressedButtons.test(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb));
	}

	bool InputHandler::WasKeyPressed(KeyboardButton kb) const
	{
		assert(kb != KeyboardButton::UNKNOWN);
		return m_PressedButtons.test(2 * static_cast<size_t>(kb) + 1);
	}

	bool InputHandler::WasKeyPressed(MouseButton mb) const
	{
		assert(mb != MouseButton::UNKNOWN);
		return m_PressedButtons.test(2 * KeyboardButtonCount + 2 * static_cast<size_t>(mb) + 1);
	}

	void InputHandler::OnMouseMove(float dx, float dy)
	{
		m_MouseMovevement = { dx, dy };
	}

	void InputHandler::Update(float dt)
	{
		// We are probably already processed the mouse movement info and don't need it anymore
		// That's why we are setting to zero like we didn't get the mouse movement info from OS during the last frame
		m_MouseMovevement = { 0.0, 0.0 };
	}
}