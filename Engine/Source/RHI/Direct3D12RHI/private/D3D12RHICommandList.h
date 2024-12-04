#pragma once

#include <d3d12.h>

#include <RHICore.h>
#include <RHICommandList.h>
#include <D3D12RHICommandAllocator.h>
#include <D3D12RHIDevice.h>
#include <D3D12RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHICommandList final : public RHICommandList
		{
		public:
			using Ptr = RefCountPtr<D3D12RHICommandList>;

		public:
			D3D12RHICommandList() = delete;
			D3D12RHICommandList(D3D12RHIDevice::Ptr device);
			~D3D12RHICommandList() = default;

		public:
			virtual void ClearRenderTarget(RHITexture::Ptr renderTarget, RenderCore::Color& color) override;
			virtual void ClearDepthStencilView(RHITexture::Ptr depthStencil, ClearFlags::Flag clearFlags, float depth, uint8_t stencil) override;
			virtual void SetRenderTargets(uint32_t TargetsNum, RHITexture::Ptr renderTarget, RHITexture::Ptr depthStencil) override;
			virtual void SetViewport(const Viewport& viewport) override;
			virtual void SetScissorRect(const Rect& scissorRect) override;
			virtual void Close() override;
			virtual void Reset() override;

			virtual void SetPipelineStateObject(RHIPipelineStateObject::Ptr pso) override;
			virtual void SetMesh(RHIMesh::Ptr mesh) override;
			virtual void SetGraphicsConstantBuffer(uint32_t ParameterIdx, RHIBuffer::Ptr buffer, uint32_t bufferOffset = 0) override;
			virtual void SetTechnique(RHITechnique::Ptr technique) override;
			virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;

			virtual void DrawIndexedInstanced(
				uint32_t IndexCountPerInstance,
				uint32_t InstanceCount,
				uint32_t StartIndexLocation,
				int32_t BaseVertexLocation,
				uint32_t StartInstanceLocation
			) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12GraphicsCommandList> GetHandle() const;

		private:
			RefCountPtr<ID3D12GraphicsCommandList> m_NativeCommandList = nullptr;

		public:
			virtual RHICommandAllocator::Ptr GetAllocator() const override;

		private:
			D3D12RHICommandAllocator::Ptr m_CommandAllocator = nullptr;

		private:
			// Move it out of here
			// Be cautious, the param is not const
			void ValidateTextureState(D3D12RHITexture* texture, D3D12_RESOURCE_STATES state);
		};
	}
}