#pragma once

#include <RHIContext.h>

namespace cqe
{
	namespace Render
	{
		class RenderPass
		{
		public:
			RenderPass() = delete;
			RenderPass(RHI::Context* rhi) : m_rhi(rhi) { }
			virtual ~RenderPass() = default;

		public:
			virtual void Render() = 0;
			virtual void Resize() = 0;

		protected:
			RHI::Context* m_rhi;
		};
	}
}