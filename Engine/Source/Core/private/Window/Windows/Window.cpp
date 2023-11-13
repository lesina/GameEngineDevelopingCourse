#include <Window.h>
#include <Window/IWindow.h>

namespace GameEngine::Core
{
	void IWindow::SetWindowHandle(void* windowHandle)
	{
		m_wndHndl = windowHandle;
	}
}