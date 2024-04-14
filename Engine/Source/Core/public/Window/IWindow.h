#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	class CORE_API Window final
	{
	public:
		Window() = default;

		void Init(void* instance);

		void* GetWindowHandle() const { return m_WndHndl; }

		uint16_t GetWidth() const { return m_Width; }
		uint16_t GetHeight() const { return m_Height; }
		void Resize(uint16_t newWidth, uint16_t newHeight) { m_Width = newWidth; m_Height = newHeight; }
		float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

	private:
		uint16_t m_Width = 800;
		uint16_t m_Height = 600;

		void* m_WndHndl = nullptr;
	};

	extern CORE_API Window* MainWindowsApplication;
}