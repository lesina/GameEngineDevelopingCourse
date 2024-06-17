#pragma once

#include <Input/Buttons.h>

namespace GameEngine
{
	namespace Core
	{
		KeyboardButton VKToKeyboardButton(uint64_t vk)
		{
			switch (vk)
			{
			case VK_LEFT:
				return KeyboardButton::LEFT;
			case VK_RIGHT:
				return KeyboardButton::RIGHT;
			case VK_UP:
				return KeyboardButton::UP;
			case VK_DOWN:
				return KeyboardButton::DOWN;
			case 'A':
				return KeyboardButton::A;
			case 'S':
				return KeyboardButton::S;
			case 'W':
				return KeyboardButton::W;
			case 'D':
				return KeyboardButton::D;
			case VK_SPACE:
				return KeyboardButton::SPACEBAR;
			case VK_F2:
				return KeyboardButton::F2;
			default:
				return KeyboardButton::UNKNOWN;
			}
		}

		MouseButton MKToMouseButton(uint64_t mk)
		{
			switch (mk)
			{
			case MK_LBUTTON:
				return MouseButton::LMB;
			default:
				return MouseButton::UNKNOWN;
			}
		}
	}
}