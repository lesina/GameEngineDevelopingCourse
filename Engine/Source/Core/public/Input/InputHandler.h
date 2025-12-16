#pragma once

#include <Core/export.h>
#include <FileSystem.h>
#include <Input/Buttons.h>
#include <Vector.h>

namespace GameEngine::Core
{
	// This is just a layer between a controller and OS-specific commands to determine which buttons are pressed
	// That is why it is singleton
	// Basically, can be replaced with SDL
	class CORE_API InputHandler final
	{
	public:
		struct MouseMovevement
		{
			float dx;
			float dy;
		};

	private:
		InputHandler();
		~InputHandler() = default;

	public:
		static InputHandler* GetInstance();

	public:
		void Update(float dt);

		void KeyPressed(KeyboardButton kb);
		void SetWasKeyPressed(KeyboardButton mb);
		void KeyReleased(KeyboardButton kb);
		void KeyPressed(MouseButton kb);
		void SetWasKeyPressed(MouseButton mb);
		void KeyReleased(MouseButton mb);
		bool IsKeyPressed(KeyboardButton kb) const;
		bool IsKeyPressed(MouseButton mb) const;
		bool WasKeyPressed(KeyboardButton kb) const;
		bool WasKeyPressed(MouseButton mb) const;
		void OnMouseMove(float dx, float dy);

		const MouseMovevement& GetMouseMovement() const { return m_MouseMovevement; }

	private:
		MouseMovevement m_MouseMovevement;

		// First half of the bitset is for "is pressed" states, second half is for "was pressed" states
		// [ KB_pressed[0] KB_was_pressed[0] KB_pressed[1] KB_was_pressed[1] ... MB_pressed[0] MB_was_pressed[0] MB_pressed[1] MB_was_pressed[1] ... ]
		std::bitset<
			2 * KeyboardButtonCount +
			2 * MouseButtonCount
		> m_PressedButtons;

		static InputHandler* m_Instance;
	};
}