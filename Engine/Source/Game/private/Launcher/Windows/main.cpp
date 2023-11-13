#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <Window/IWindow.h>
#include <Game.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    GameEngine::Core::IWindow* window = new GameEngine::Core::IWindow();
    window->SetWindowHandle(hInstance);

    std::unique_ptr<GameEngine::Game> game = std::make_unique<GameEngine::Game>(window);

    game->Run();

    MSG msg = {};

    return (int)msg.wParam;
}