#pragma once

#include <RHIContext.h>

namespace cqe
{
	namespace Render
	{
		class RenderFeature;
		class RenderObject;

		class RenderPipeline final
		{
		public:
			RenderPipeline() = delete;
			RenderPipeline(
				const std::vector<RenderObject*>& renderObjects,
				std::shared_ptr<RHI::Context> rhi
			);
			~RenderPipeline();

		public:
			void Render();
			void Resize();

		private:
			std::vector<RenderFeature*> m_Features;
			RHI::Context* m_rhi;
		};
	}
}