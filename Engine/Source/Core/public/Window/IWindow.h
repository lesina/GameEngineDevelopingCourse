#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	class CORE_API IWindow final
	{
	public:
		virtual void SetWindowHandle(void* windowHandle);

	private:
		void* m_wndHndl = nullptr;
	};
}