#pragma once

#include <Geometry.h>
#include <RenderEngine/export.h>
#include <RenderObject.h>
#include <Window/IWindow.h>

namespace cqe::Render
{
	namespace RHI
	{
		class Context;
	}

	class RenderPipeline;

	class RENDER_ENGINE_API RenderEngine final
	{
	public:
		RenderEngine();
		~RenderEngine();

		void Update();
		void OnResize();

		void CreateRenderObject(RenderCore::Geometry* geometry, RenderObject* renderObject);

		std::shared_ptr<RHI::Context> GetRHI() const { return m_rhi; }

	private:
		RenderPipeline* m_RenderPipeline = nullptr;
		std::vector<RenderObject*> m_RenderObjects;

		std::shared_ptr<RHI::Context> m_rhi;
	};
}