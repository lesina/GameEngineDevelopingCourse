#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	HWND GetPlatformWindowHandle(void* handle)
	{
		return reinterpret_cast<HWND>(handle);
	}
}