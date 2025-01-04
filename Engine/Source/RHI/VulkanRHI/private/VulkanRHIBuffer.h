#pragma once

#include <VulkanRHIDevice.h>
#include <RHIBuffer.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIBuffer final : public RHIBuffer
		{
		public:
			using Ptr = RefCountPtr<VulkanRHIBuffer>;

		public:
			VulkanRHIBuffer() = delete;
			VulkanRHIBuffer(
				const Description& desc,
				VulkanRHIDevice::Ptr device,
				VkDeviceMemory memory,
				VkBuffer buffer
			);

			~VulkanRHIBuffer() override;

		public:
			virtual void CopyData(int elementIndex, void* data, uint32_t dataSize) override;
			virtual RenderNativeObject GetNativeObject() override;
			VkBuffer GetHandle() const;

			uint32_t GetSize() const;

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;

			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			VkBuffer m_Buffer = VK_NULL_HANDLE;
		};
	}
}