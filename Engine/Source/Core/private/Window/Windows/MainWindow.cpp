#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>

#include <Window.h>
#include <WindowEventsCallbacks.h>
#include <Window/IWindow.h>

namespace GameEngine::Core
{
	Window* g_MainWindowsApplication = nullptr;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
			// Save the new client area dimensions.
			g_MainWindowsApplication->Resize(LOWORD(lParam), HIWORD(lParam));
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), g_MainWindowsApplication);
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), g_MainCamera, g_MainWindowsApplication);
			return 0;
		case WM_KEYDOWN:
		case WM_KEYLAST:
			OnKeyDown(wParam, g_MainCamera, g_MainWindowsApplication);
			return 0;
		case WM_KEYUP:
			OnKeyUp(wParam, g_MainCamera);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::Init(void* instance)
	{
		HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(instance);

		std::wstring windowName = L"Game";
		std::wstring className = L"GameWindow";

		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = windowName.c_str();
		wc.lpszClassName = className.c_str();
		wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return;
		}

		DWORD borderlessStyle = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		DWORD defaultStyle = WS_OVERLAPPEDWINDOW;

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, (long)m_Width, (long)m_Height };
		AdjustWindowRect(&R, defaultStyle, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_WndHndl = CreateWindow(className.c_str(), windowName.c_str(),
			defaultStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
		if (!m_WndHndl)
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
			return;
		}

		ShowWindow(GetPlatformWindowHandle(m_WndHndl), SW_SHOW);
		UpdateWindow(GetPlatformWindowHandle(m_WndHndl));

		return;
	}
}