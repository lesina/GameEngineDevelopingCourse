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
		void KeyReleased(KeyboardButton kb);
		void KeyPressed(MouseButton kb);
		void KeyReleased(MouseButton mb);
		bool IsKeyPressed(KeyboardButton kb) const;
		bool IsKeyPressed(MouseButton mb) const;
		void OnMouseMove(float dx, float dy);

		const MouseMovevement& GetMouseMovement() const { return m_MouseMovevement; }

	private:
		MouseMovevement m_MouseMovevement;

		std::bitset<
			KeyboardButtonCount +
			MouseButtonCount
		> m_PressedButtons;

		static InputHandler* m_Instance;
	};
}