#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		enum class KeyboardButton : uint16_t
		{
			A = 0,
			S,
			W,
			D,
			UP,
			DOWN,
			LEFT,
			RIGHT,

			UNKNOWN
		};

		enum class MouseButton : uint16_t
		{
			LMB = 0,

			UNKNOWN
		};

		extern CORE_API const std::unordered_map<std::string, KeyboardButton> KeyboardButtonsMap;
		extern CORE_API const std::unordered_map<std::string, MouseButton> MouseButtonsMap;

		KeyboardButton CORE_API StringToKeyboardButton(const std::string& key);
		MouseButton CORE_API StringToMouseButton(const std::string& key);
	}
}