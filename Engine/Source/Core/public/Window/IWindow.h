#pragma once

#include <Core/export.h>
#include <Vector.h>
#include <PackedVariables.h>

namespace GameEngine::Core
{
	class CORE_API Window
	{
	public:
		using Ptr = std::unique_ptr<Window>;

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
		
		void Update();

		void CaptureMouse();
		void ReleaseMouse();

		void Focus();
		void UnFocus();

		inline bool IsMouseCaptured() const { return m_IsMouseCaptured; }
		inline bool IsFocused() const { return m_IsFocused; }
		inline bool IsWindowed() const { return m_IsWindowed; }

	private:
		uint32_t m_Width = 800;
		uint32_t m_Height = 600;

		void* m_WndHndl = nullptr;

		// In screen coords
		Math::Vector2i m_MousePos;

		bool m_IsMouseCaptured = false;
		bool m_IsFocused = false;
		bool m_IsWindowed = true;
	};

	extern CORE_API std::function<bool(PackedVariables&)> g_GUIWindowsCallback;
	extern CORE_API Window::Ptr g_MainWindowsApplication;
}