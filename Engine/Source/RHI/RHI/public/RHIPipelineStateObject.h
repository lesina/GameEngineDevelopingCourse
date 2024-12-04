#pragma once

#include <RHICommon.h>
#include <RHICore.h>
#include <RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHIPipelineStateObject : public RefCounter<RenderBackendResource>
		{
		public:
			struct Description
			{
				RHITechnique::Ptr Technique = nullptr;
				BlendDescription BlendState;
				RasterizerDescription RasterState;
				DepthStencilDescription DepthStencilState;
				PrimitiveTopologyType PrimitiveTopology = PrimitiveTopologyType::Triangle;
				uint32_t NumRenderTargets = 1;
				ResourceFormat RTVFormats[8];
				ResourceFormat DSVFormat;
			};

		public:
			using Ptr = RefCountPtr<RHIPipelineStateObject>;

		public:
			RHIPipelineStateObject() = delete;
			RHIPipelineStateObject(const Description& desc) : m_Description(desc) {}

		protected:
			Description m_Description;
		};
	}
}