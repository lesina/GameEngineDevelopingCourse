#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	class CORE_API Window final
	{
	public:
		using Ptr = std::shared_ptr<Window>;

	public:
		Window() = delete;
		Window(void* instance);

		void* GetWindowHandle() const { return m_wndHndl; }

		uint16_t GetWidth() const { return m_width; }
		uint16_t GetHeight() const { return m_height; }
		float GetAspectRatio() const { return (float)m_width / (float)m_height; }

	private:
		uint16_t m_width = 800;
		uint16_t m_height = 600;

		void* m_wndHndl = nullptr;
	};
}