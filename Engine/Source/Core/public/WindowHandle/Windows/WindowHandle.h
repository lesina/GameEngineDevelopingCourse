#include <WindowHandle/IWindowHandle.h>

class CORE_API WindowHandle final : public IWindowHandle
{
public:
	virtual void SetWindowHandle(void* windowHandle) override;
private:
	void* m_WindowHandle;
};