#pragma once

#include <d3d12.h>

#include <RHICore.h>
#include <CommandList.h>
#include <D3D12CommandAllocator.h>
#include <D3D12Device.h>
#include <D3D12Texture.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12CommandList final : public CommandList
		{
		public:
			using Ptr = RefCountPtr<D3D12CommandList>;

		public:
			D3D12CommandList() = delete;
			D3D12CommandList(D3D12Device::Ptr device);
			~D3D12CommandList() = default;

		public:
			virtual void ClearRenderTarget(Texture::Ptr renderTarget, RenderCore::Color& color) override;
			virtual void ClearDepthStencilView(Texture::Ptr depthStencil, ClearFlags::Flag clearFlags, float depth, uint8_t stencil) override;
			virtual void SetRenderTargets(uint32_t TargetsNum, Texture::Ptr renderTarget, Texture::Ptr depthStencil) override;
			virtual void SetViewport(const Viewport& viewport) override;
			virtual void SetScissorRect(const Rect& scissorRect) override;
			virtual void Close() override;
			virtual void Reset() override;

			virtual void SetPipelineStateObject(PipelineStateObject::Ptr pso) override;
			virtual void SetMesh(Mesh::Ptr mesh) override;
			virtual void SetGraphicsDescriptorTableForTexture(uint32_t parameterIdx, Texture* texture) override;
			virtual void SetGraphicsConstantBuffer(uint32_t ParameterIdx, Buffer::Ptr buffer, uint32_t bufferOffset = 0) override;
			virtual void SetTechnique(Technique::Ptr technique) override;
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
			virtual CommandAllocator::Ptr GetAllocator() const override;

		private:
			D3D12CommandAllocator::Ptr m_CommandAllocator = nullptr;

		private:
			// Move it out of here
			// Be cautious, the param is not const
			void ValidateTextureState(D3D12Texture* texture, D3D12_RESOURCE_STATES state);
		};
	}
}