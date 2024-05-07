#pragma once

#include <Core/export.h>
#include <Vector.h>

namespace GameEngine::Core
{
	class CORE_API Window final
	{
	public:
		Window() = default;

		void Init(void* instance);

		void* GetWindowHandle() const { return m_WndHndl; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		void Resize(uint32_t newWidth, uint32_t newHeight) { m_Width = newWidth; m_Height = newHeight; }
		float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }
		Math::Vector2i GetMousePos() const { return m_MousePos; }
		void SetMousePos(int x, int y) { m_MousePos.x = x; m_MousePos.y = y; }

	private:
		uint32_t m_Width = 800;
		uint32_t m_Height = 600;

		void* m_WndHndl = nullptr;

		Math::Vector2i m_MousePos;
	};

	extern CORE_API Window* g_MainWindowsApplication;
}