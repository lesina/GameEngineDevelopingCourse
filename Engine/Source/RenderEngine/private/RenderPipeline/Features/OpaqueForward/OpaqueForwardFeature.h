#pragma once

#include <RenderPipeline/Features/RenderFeature.h>

namespace cqe
{
	namespace Render
	{
		class RenderObject;

		class OpaqueForwardFeature final : public RenderFeature
		{
		public:
			OpaqueForwardFeature() = delete;
			OpaqueForwardFeature(const std::vector<RenderObject*>& renderObjects, RHI::Context* rhi);
			virtual ~OpaqueForwardFeature() = default;

		public:
			virtual void Render() override;
		};
	}
}