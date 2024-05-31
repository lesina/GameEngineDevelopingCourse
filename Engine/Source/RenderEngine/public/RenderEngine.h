#pragma once

#include <Geometry.h>
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
		RenderEngine();

		void Update(size_t frame);
		void OnResize();

		void CreateRenderObject(RenderCore::Geometry::Ptr geometry, RenderObject* renderObject);

	private:
		uint32_t m_swapChainWidth;
		uint32_t m_swapChainHeight;

		std::vector<RenderObject*> m_RenderObjects;

		std::shared_ptr<HAL::RHIAdapter> m_rhi;
	};
}