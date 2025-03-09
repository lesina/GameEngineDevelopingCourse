#include <RenderPipeline/Features/RenderFeature.h>
#include <RenderPipeline/Features/RenderPass.h>

namespace cqe
{
	namespace Render
	{
		RenderFeature::~RenderFeature()
		{
			for (RenderPass* renderPass : m_RenderPasses)
			{
				delete renderPass;
			}
		}

		void RenderFeature::Resize()
		{
			for (RenderPass* renderPass : m_RenderPasses)
			{
				renderPass->Resize();
			}
		}
	}
}