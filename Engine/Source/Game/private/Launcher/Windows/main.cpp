#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <Window/IWindow.h>
#include <Game.h>
#include <array.h>
#include <iostream>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

bool WindowsMessageLoop()
{
	MSG msg = { 0 };

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == (WM_QUIT | WM_CLOSE))
		{
			return false;
		}
	}

	return msg.message != (WM_QUIT | WM_CLOSE);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameEngine::Core::MainWindowsApplication = new GameEngine::Core::Window();
	GameEngine::Core::MainWindowsApplication->Init(hInstance);

	std::unique_ptr<GameEngine::Game> game = std::make_unique<GameEngine::Game>(&WindowsMessageLoop);

	game->Run();

	return 0;
}