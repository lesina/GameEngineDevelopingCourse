#include <RenderPipeline/Features/OpaqueForward/OpaqueForwardFeature.h>

#include <RenderPipeline/Features/OpaqueForward/Passes/ForwardPass.h>

namespace cqe
{
	namespace Render
	{
		OpaqueForwardFeature::OpaqueForwardFeature(
			const std::vector<RenderObject*>& renderObjects,
			RHI::Context* rhi
		) : RenderFeature(rhi)
		{
			assert(rhi);

			ForwardPass* forwardPass = new ForwardPass(renderObjects, rhi);

			m_RenderPasses =
			{
				forwardPass
			};
		}

		void OpaqueForwardFeature::Render()
		{
			for (RenderPass* renderPass : m_RenderPasses)
			{
				renderPass->Render();
			}
		}
	}
}