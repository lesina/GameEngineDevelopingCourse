#pragma once

#include <Geometry.h>
#include <RHIContext.h>
#include <RHI/VulkanRHI/export.h>
#include <RHIMesh.h>
#include <RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIDevice;
		class VulkanRHIFactory;
		class VulkanRHISwapChain;
		class VulkanRHIFence;
		class VulkanRHIBuffer;
		class VulkanRHICommandQueue;
		class VulkanRHICommandList;
		class VulkanShaderCompiler;

		class VULKAN_API VulkanRHIContext final : public RHIContext
		{
		public:
			VulkanRHIContext();
			~VulkanRHIContext();

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
			void RecreateTransferStagingBuffer(uint32_t size);

		private:
			std::unique_ptr<VulkanShaderCompiler> m_ShaderCompiler = nullptr;

			RefCountPtr<VulkanRHIFactory> m_Factory = nullptr;
			RefCountPtr<VulkanRHIDevice> m_Device = nullptr;
			RefCountPtr<VulkanRHISwapChain> m_SwapChain = nullptr;
			RefCountPtr<VulkanRHIFence> m_Fence = nullptr;
			RefCountPtr<VulkanRHICommandQueue> m_CommandQueue = nullptr;
			RefCountPtr<VulkanRHICommandList> m_CommandList = nullptr;

			RefCountPtr<RHIBuffer> m_TransferStagingBuffer = nullptr;
			RefCountPtr<VulkanRHICommandList> m_TransferCommandList = nullptr;
		};
	}
}