#include <VulkanRHIFence.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHIFence::VulkanRHIFence(VulkanRHIDevice::Ptr device)
			: m_Device(device)
		{
		}

		void VulkanRHIFence::Sync(RHICommandQueue::Ptr commandQueue)
		{
			VULKAN_CALL_CHECK(vkQueueWaitIdle(commandQueue->GetNativeObject()));
		}

		RenderNativeObject VulkanRHIFence::GetNativeObject()
		{
			return RenderNativeObject(VK_NULL_HANDLE);
		}
	}
}