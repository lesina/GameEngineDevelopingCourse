#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>

#include <Input/InputHandler.h>
#include <Input/Windows/WindowsKeyboardButtons.h>
#include <Window.h>
#include <Window/IWindow.h>

namespace GameEngine::Core
{
	Window::Ptr g_MainWindowsApplication = nullptr;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
			g_MainWindowsApplication->Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
			InputHandler::GetInstance()->KeyPressed(VKToKeyboardButton(wParam));
			return 0;
		case WM_KEYUP:
			InputHandler::GetInstance()->KeyReleased(VKToKeyboardButton(wParam));
			return 0;
		case WM_MOUSEMOVE:
			if (!g_MainWindowsApplication->IsFocused()) [[unlikely]]
			{
				return 0;
			}

			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			ClientToScreen(GetPlatformWindowHandle(g_MainWindowsApplication->GetWindowHandle()), &pt);
			Math::Vector2i pos = g_MainWindowsApplication->GetMousePos();
			InputHandler::GetInstance()->OnMouseMove(pt.x - pos.x, pt.y - pos.y);
			return 0;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			if (g_MainWindowsApplication->IsFocused()) [[likely]]
			{
				InputHandler::GetInstance()->KeyPressed(MKToMouseButton(wParam));
			}
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			if (g_MainWindowsApplication->IsFocused()) [[likely]]
			{
				InputHandler::GetInstance()->KeyReleased(MKToMouseButton(wParam));
			}
			return 0;
		case WM_SETFOCUS:
			g_MainWindowsApplication->Focus();
			return 0;
		case WM_KILLFOCUS:
			g_MainWindowsApplication->UnFocus();
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::Init(void* instance)
	{
		m_InstanceHndl = instance;

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

	void Window::Update()
	{
		if (m_IsFocused) [[likely]]
		{
			RECT R;
			GetWindowRect(GetPlatformWindowHandle(GetWindowHandle()), &R);

			POINT pt;
			pt.x = R.left + (R.right - R.left) / 2;
			pt.y = R.top + (R.bottom - R.top) / 2;

			SetCursorPos(pt.x, pt.y);
			SetMousePos(pt.x, pt.y);
		}
	}

	void Window::Focus()
	{
		SetCapture(GetPlatformWindowHandle(GetWindowHandle()));
		SetFocus(GetPlatformWindowHandle(GetWindowHandle()));
		ShowCursor(false);

		m_IsFocused = true;
	}

	void Window::UnFocus()
	{
		ReleaseCapture();
		SetFocus(nullptr);
		ShowCursor(true);

		m_IsFocused = false;
	}
}