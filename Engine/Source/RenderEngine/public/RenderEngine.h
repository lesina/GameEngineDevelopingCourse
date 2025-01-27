#pragma once

#include <Geometry.h>
#include <RenderEngine/export.h>
#include <RenderObject.h>
#include <Window/IWindow.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHIContext;
	}

	class RENDER_ENGINE_API RenderEngine final
	{
	public:
		RenderEngine();
		~RenderEngine();

		void Update(size_t frame);
		void OnResize();

		void CreateRenderObject(RenderCore::Geometry* geometry, RenderObject* renderObject);

		std::shared_ptr<HAL::RHIContext> GetRHI() const { return m_rhi; }

	private:
		std::vector<RenderObject*> m_RenderObjects;

		std::shared_ptr<HAL::RHIContext> m_rhi;
	};
}