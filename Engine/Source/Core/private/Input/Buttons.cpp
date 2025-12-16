#include <Input/Buttons.h>

namespace GameEngine::Core
{
	const std::unordered_map<std::string, KeyboardButton> KeyboardButtonsMap =
	{
		{"left", KeyboardButton::LEFT},
		{"right", KeyboardButton::RIGHT},
		{"up", KeyboardButton::UP},
		{"down", KeyboardButton::DOWN},
		{"a", KeyboardButton::A},
		{"w", KeyboardButton::W},
		{"s", KeyboardButton::S},
		{"d", KeyboardButton::D},
		{"q", KeyboardButton::Q},
		{"e", KeyboardButton::E},
		{"c", KeyboardButton::C},
		{"f", KeyboardButton::F},
		{"t", KeyboardButton::T},
		{"r", KeyboardButton::R},
		{"spacebar", KeyboardButton::SPACEBAR},
		{"f2", KeyboardButton::F2}
	};

	KeyboardButton StringToKeyboardButton(const std::string& key)
	{
		assert(KeyboardButtonsMap.contains(key));

		return KeyboardButtonsMap.at(key);
	}

	const std::unordered_map<std::string, MouseButton> MouseButtonsMap =
	{
		{"lmb", MouseButton::LMB},
		{"rmb", MouseButton::RMB}
	};

	MouseButton StringToMouseButton(const std::string& key)
	{
		assert(MouseButtonsMap.contains(key));

		return MouseButtonsMap.at(key);
	}
}