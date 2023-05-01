#pragma once

#include <Core/export.h>

class CORE_API IWindowHandle
{
public:
	virtual void SetWindowHandle(void* windowHandle) = 0;
	//Set
};