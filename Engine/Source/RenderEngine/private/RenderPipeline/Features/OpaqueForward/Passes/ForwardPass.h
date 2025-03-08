#pragma once

#include <RenderPipeline/Features/RenderPass.h>
#include <RHIContext.h>
#include <Matrix.h>
#include <RenderCore.h>
#include <RenderObject.h>

namespace cqe
{
	namespace Render
	{
		class ForwardPass final : public RenderPass
		{
		public:
			ForwardPass() = delete;
			ForwardPass(const std::vector<RenderObject*>& renderObjects, RHI::Context* rhi);
			virtual ~ForwardPass() = default;

		public:
			virtual void Render() override;
			virtual void Resize() override;

		private:
			struct ObjectConstants
			{
				Math::Matrix4x4f WorldViewProj = Math::Matrix4x4f::Identity();
			};

			struct MaterialConstants
			{
				Math::Vector4f Color = Math::Vector4f::Zero();
			};

			RHI::Buffer::Ptr m_ObjectCB[RenderCore::g_FrameBufferCount];
			RHI::Buffer::Ptr m_MaterialCB[RenderCore::g_FrameBufferCount];
			RHI::Technique::Ptr m_Technique;
			RHI::PipelineStateObject::Ptr m_PSO;
			RHI::Texture::Ptr m_DepthStencil;

			// And that's the problems you get when you don't have a proper architecture
			const std::vector<RenderObject*>& m_RenderObjects;
		};
	}
}