#pragma once

#include <Geometry.h>
#include <RHIContext.h>
#include <RHI/Direct3D12RHI/export.h>
#include <RHIMesh.h>
#include <RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIDevice;
		class D3D12RHIFactory;
		class D3D12RHISwapChain;
		class D3D12RHIFence;
		class D3D12RHICommandQueue;
		class D3D12RHICommandAllocator;
		class D3D12RHICommandList;
		class D3D12DescriptorHeap;

		class DIRECT3D_API D3D12RHIContext final : public RHIContext
		{
		public:
			D3D12RHIContext();
			~D3D12RHIContext();

		public:
			virtual RHITexture::Ptr CreateTexture(const RHITexture::Description& description) override;
			virtual RHIBuffer::Ptr CreateBuffer(RHIBuffer::Description&& description) override;
			virtual RHITechnique::Ptr CreateTechnique(
				const RHITechnique::ShaderInfo& shaderInfo,
				const RHITechnique::InputLayout& inputLayout,
				const RHITechnique::RootSignature& rootSignature
			) override;
			virtual RHIPipelineStateObject::Ptr CreatePSO(const RHIPipelineStateObject::Description& description) override;
			virtual RHIMesh::Ptr CreateMesh(
				const RHIMesh::VertexBufferDescription& vertexDesc,
				const RHIMesh::IndexBufferDescription& indexDesc
			) override;

		public:
			virtual RHIDevice::Ptr GetDevice() const override;
			virtual RHIFactory::Ptr GetFactory() const override;
			virtual RHISwapChain::Ptr GetSwapChain() const override;
			virtual RHIFence::Ptr GetFence() const override;
			virtual RHICommandQueue::Ptr GetCommandQueue() const override;
			virtual RHICommandList::Ptr GetCommandList() const override;

		private:
			void EnableDebugLayer() const;

		private:
			RefCountPtr<D3D12RHIDevice> m_Device = nullptr;
			RefCountPtr<D3D12RHIFactory> m_Factory = nullptr;
			RefCountPtr<D3D12RHISwapChain> m_SwapChain = nullptr;
			RefCountPtr<D3D12RHIFence> m_Fence = nullptr;
			RefCountPtr<D3D12RHICommandQueue> m_CommandQueue = nullptr;
			RefCountPtr<D3D12RHICommandList> m_CommandList = nullptr;

			RefCountPtr<D3D12DescriptorHeap> m_RtvHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_DsvHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_SrvCbvUavHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_SamplerHeap = nullptr;
		};
	}
}