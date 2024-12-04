#pragma once

#include <Colors.h>
#include <RHICommon.h>
#include <RHICommandAllocator.h>
#include <RHIPipelineStateObject.h>
#include <RHITexture.h>
#include <RHIMesh.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHICommandList : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHICommandList>;

		public:
			virtual void ClearRenderTarget(RHITexture::Ptr renderTarget, RenderCore::Color& color) = 0;
			virtual void ClearDepthStencilView(RHITexture::Ptr depthStencil, ClearFlags::Flag clearFlag, float depth, uint8_t stencil) = 0;
			virtual void SetRenderTargets(uint32_t TargetsNum, RHITexture::Ptr renderTarget, RHITexture::Ptr depthStencil) = 0;
			virtual void SetViewport(const Viewport& viewport) = 0;
			virtual void SetScissorRect(const Rect& scissorRect) = 0;
			virtual void Close() = 0;
			virtual void Reset() = 0;

			virtual void SetPipelineStateObject(RHIPipelineStateObject::Ptr pso) = 0;
			virtual void SetMesh(RHIMesh::Ptr mesh) = 0;
			virtual void SetGraphicsConstantBuffer(uint32_t ParameterIdx, RHIBuffer::Ptr buffer, uint32_t bufferOffset = 0) = 0;
			virtual void SetTechnique(RHITechnique::Ptr technique) = 0;
			virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
			
			virtual void DrawIndexedInstanced(
				uint32_t IndexCountPerInstance,
				uint32_t InstanceCount,
				uint32_t StartIndexLocation,
				int32_t BaseVertexLocation,
				uint32_t StartInstanceLocation
			) = 0;

		public:
			virtual RHICommandAllocator::Ptr GetAllocator() const = 0;
		};
	}
}