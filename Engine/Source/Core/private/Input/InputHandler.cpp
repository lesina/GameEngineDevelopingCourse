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

		m_PressedButtons.set(static_cast<size_t>(kb), true);
	}

	void InputHandler::KeyReleased(KeyboardButton kb)
	{
		// Delete this logic when all the buttons
		if (kb == KeyboardButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(static_cast<size_t>(kb), false);
	}

	void InputHandler::KeyPressed(MouseButton mb)
	{
		// Delete this logic when all the buttons
		if (mb == MouseButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(KeyboardButtonCount + static_cast<size_t>(mb), true);
	}

	void InputHandler::KeyReleased(MouseButton mb)
	{
		// Delete this logic when all the buttons
		if (mb == MouseButton::UNKNOWN) [[unlikely]]
		{
			return;
		}

		m_PressedButtons.set(KeyboardButtonCount + static_cast<size_t>(mb), false);
	}

	bool InputHandler::IsKeyPressed(KeyboardButton kb) const
	{
		assert(kb != KeyboardButton::UNKNOWN);
		return m_PressedButtons.test(static_cast<size_t>(kb));
	}

	bool InputHandler::IsKeyPressed(MouseButton mb) const
	{
		assert(mb != MouseButton::UNKNOWN);
		return m_PressedButtons.test(KeyboardButtonCount + static_cast<size_t>(mb));
	}

	void InputHandler::OnMouseMove(float dx, float dy)
	{
		dx *= 0.25 * Math::Constants::PI / 180.f;
		dy *= 0.25 * Math::Constants::PI / 180.f;

		g_MainCamera->Rotate(dx, dy);
	}
}