#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shellapi.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <CommandLine.h>
#include <FileSystem.h>
#include <Game.h>
#include <Input/InputHandler.h>
#include <Window/IWindow.h>



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

void WaitForDebugger()
{
#if defined(DEBUG)
	if (GameEngine::Core::g_CommandLineArguments->HasAttribute("wfd"))
	{
		while (!::IsDebuggerPresent())
		{
			::Sleep(100);
		}
	}
#endif
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	int argc;
	LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
	const std::vector<std::wstring> args(argv, argv + argc);

	GameEngine::Core::g_CommandLineArguments = GameEngine::Core::CommandLine::Parse(args);
	assert(GameEngine::Core::g_CommandLineArguments->HasAttribute("project_root"));

	WaitForDebugger();

	GameEngine::Core::g_MainWindowsApplication = std::make_unique<GameEngine::Core::Window>();
	GameEngine::Core::g_MainWindowsApplication->Init(hInstance);

	GameEngine::Core::g_FileSystem = GameEngine::Core::FileSystem::Create(GameEngine::Core::g_CommandLineArguments->GetAttribute("project_root"));

	std::unique_ptr<GameEngine::Game> game = std::make_unique<GameEngine::Game>(&WindowsMessageLoop);

	game->Run();

	return 0;
}