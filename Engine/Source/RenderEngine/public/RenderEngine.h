#pragma once

#include <RenderEngine/export.h>
#include <RenderObject.h>
#include <Window/IWindow.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHIAdapter;
	}

	class RENDER_ENGINE_API RenderEngine final
	{
	public:
		RenderEngine() = delete;
		RenderEngine(Core::Window::Ptr window);

		void Update();
		void SetMainWindow(Core::Window::Ptr window);
		void OnResize();

	private:
		std::vector<RenderObject::Ptr> m_RenderObjects;

		std::shared_ptr<HAL::RHIAdapter> m_rhi;
	};
}