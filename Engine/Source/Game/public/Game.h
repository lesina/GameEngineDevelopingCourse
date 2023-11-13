#pragma once

#include <Window/IWindow.h>

namespace GameEngine
{
	class Game final
	{
	public:
		Game() = delete;
		Game(Core::IWindow* window);

		void Run();

	private:
		Core::IWindow* m_window;
	};
}