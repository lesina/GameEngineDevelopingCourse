#include <VulkanRHICommandAllocator.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHICommandAllocator::VulkanRHICommandAllocator(VulkanRHIDevice::Ptr device)
			: m_Device(device)
		{
			const VkCommandPoolCreateInfo poolInfo =
			{
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
				.queueFamilyIndex = m_Device->GetMainQueueFamilyIndex(),
			};

			VULKAN_CALL_CHECK(vkCreateCommandPool(m_Device->GetHandle(), &poolInfo, nullptr, &m_NativeCommandPool));
		}

		VulkanRHICommandAllocator::~VulkanRHICommandAllocator()
		{
			if (m_NativeCommandPool != VK_NULL_HANDLE)
			{
				vkDestroyCommandPool(m_Device->GetHandle(), m_NativeCommandPool, nullptr);
				m_NativeCommandPool = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHICommandAllocator::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandPool);
		}

		VkCommandPool VulkanRHICommandAllocator::GetHandle() const
		{
			return m_NativeCommandPool;
		}

		void VulkanRHICommandAllocator::Reset()
		{
			VULKAN_CALL_CHECK(vkResetCommandPool(m_Device->GetHandle(), m_NativeCommandPool, 0U));
		}
	}
}