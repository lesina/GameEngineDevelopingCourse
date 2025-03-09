#pragma once

#include <RHIContext.h>

namespace cqe
{
	namespace Render
	{
		class RenderPass;

		class RenderFeature
		{
		public:
			RenderFeature() = delete;
			RenderFeature(RHI::Context* rhi) : m_rhi(rhi) { }
			virtual ~RenderFeature();

		public:
			virtual void Render() = 0;

		public:
			void Resize();

		protected:
			std::vector<RenderPass*> m_RenderPasses;

			RHI::Context* m_rhi;
		};
	}
}