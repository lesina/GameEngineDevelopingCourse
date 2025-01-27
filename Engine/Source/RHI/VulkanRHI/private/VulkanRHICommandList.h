#pragma once

#include <RHICore.h>
#include <RHICommandList.h>
#include <VulkanRHICommandAllocator.h>
#include <VulkanRHIDevice.h>
#include <VulkanRHITexture.h>
#include <VulkanRHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHICommandList final : public RHICommandList
		{
		public:
			using Ptr = RefCountPtr<VulkanRHICommandList>;

		public:
			VulkanRHICommandList() = delete;
			VulkanRHICommandList(VulkanRHIDevice::Ptr device);
			~VulkanRHICommandList() = default;

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
			VkCommandBuffer GetHandle() const;
			uint32_t GetCommandsRecorded() const;

			void EndRenderPassIfBound();

			void CopyBufferToBuffer(RHIBuffer::Ptr src, RHIBuffer::Ptr dst, uint32_t sizeBytes);

			void ValidateTextureState(VulkanRHITexture* texture, const VulkanRHITexture::State& state);

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;
			VkCommandBuffer m_NativeCommandBuffer = VK_NULL_HANDLE;

			VulkanRHITechnique::Ptr m_BoundTechnique = nullptr;

			VulkanRHITexture::Ptr m_BoundRenderTarget = nullptr;
			VulkanRHITexture::Ptr m_BoundDepthStencil = nullptr;

			bool m_Recording = false;
			uint32_t m_CommandsRecorded = 0;

		public:
			virtual RHICommandAllocator::Ptr GetAllocator() const override;

		private:
			VulkanRHICommandAllocator::Ptr m_CommandAllocator = nullptr;

		private:
			void ResetIfNeeded();
		};
	}
}