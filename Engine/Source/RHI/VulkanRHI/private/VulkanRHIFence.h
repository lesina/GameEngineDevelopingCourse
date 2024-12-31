#pragma once

#include <RHIFence.h>
#include <VulkanRHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIFence final : public RHIFence
		{
		public:
			using Ptr = RefCountPtr<VulkanRHIFence>;

		public:
			VulkanRHIFence() = delete;
			VulkanRHIFence(VulkanRHIDevice::Ptr device);
			//~VulkanRHIFence();

		public:
			virtual void Sync(RHICommandQueue::Ptr commandQueue) override;

			virtual RenderNativeObject GetNativeObject() override;
			//VkSemaphore GetHandle() const;

		private:
			//uint64_t m_CurrentFence = 0;

			VulkanRHIDevice::Ptr m_Device = nullptr;
			//VkSemaphore m_NativeHandle = VK_NULL_HANDLE;
		};
	}
}