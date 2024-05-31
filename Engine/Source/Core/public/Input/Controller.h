#pragma once

#include <Input/Buttons.h>
#include <Core/export.h>
#include <FileSystem.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API Controller final
		{
		public:
			Controller() = delete;
			Controller(FileSystem::Path&& config);
			~Controller() = default;

		public:
			bool IsPressed(const std::string& event);

		private:
			std::unordered_map<std::string, KeyboardButton> m_KeyboardEventMap;
			std::unordered_map<std::string, MouseButton> m_MouseEventMap;

			FileSystem::Path m_Config;
		};
	}
}