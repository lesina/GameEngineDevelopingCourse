#pragma once

#include <Geometry.h>
#include <RHIContext.h>
#include <RHI/Direct3D12RHI/export.h>
#include <Mesh.h>
#include <Texture.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Device;
		class D3D12Factory;
		class D3D12SwapChain;
		class D3D12Fence;
		class D3D12CommandQueue;
		class D3D12CommandAllocator;
		class D3D12CommandList;
		class D3D12DescriptorHeap;

		class DIRECT3D_API D3D12Context final : public Context
		{
		public:
			D3D12Context();
			~D3D12Context();

		public:
			virtual Texture* CreateTexture(const Texture::Description& description) override;
			virtual void FreeTexture(const Texture* texture, Texture::UsageFlags::Flag usageFlag) override;
			virtual void LoadTextureFromFile(Texture* texture, const std::wstring&& filePath) override;
			virtual Buffer::Ptr CreateBuffer(Buffer::Description&& description) override;
			virtual Technique::Ptr CreateTechnique(
				const Technique::ShaderInfo& shaderInfo,
				const Technique::InputLayout& inputLayout,
				const Technique::RootSignatureDescription& rootSignatureDescriptor
			) override;
			virtual PipelineStateObject::Ptr CreatePSO(const PipelineStateObject::Description& description) override;
			virtual Mesh::Ptr CreateMesh(
				const Mesh::VertexBufferDescription& vertexDesc,
				const Mesh::IndexBufferDescription& indexDesc
			) override;
			virtual void SetDescriptorHeaps() override;
			virtual bool CheckMinimalRequirements() const override;

		public:
			virtual Device::Ptr GetDevice() const override;
			virtual Factory::Ptr GetFactory() const override;
			virtual SwapChain::Ptr GetSwapChain() const override;
			virtual Fence::Ptr GetFence() const override;
			virtual CommandQueue::Ptr GetCommandQueue() const override;
			virtual CommandList::Ptr GetCommandList() const override;

		private:
			void EnableDebugLayer() const;

		public:
			RefCountPtr<D3D12DescriptorHeap> GetSrvHeap() const;

		private:
			RefCountPtr<D3D12Device> m_Device = nullptr;
			RefCountPtr<D3D12Factory> m_Factory = nullptr;
			RefCountPtr<D3D12SwapChain> m_SwapChain = nullptr;
			RefCountPtr<D3D12Fence> m_Fence = nullptr;
			RefCountPtr<D3D12CommandQueue> m_CommandQueue = nullptr;
			RefCountPtr<D3D12CommandList> m_CommandList = nullptr;

			RefCountPtr<D3D12DescriptorHeap> m_RtvHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_DsvHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_SrvCbvUavHeap = nullptr;
			RefCountPtr<D3D12DescriptorHeap> m_SamplerHeap = nullptr;
		};
	}
}