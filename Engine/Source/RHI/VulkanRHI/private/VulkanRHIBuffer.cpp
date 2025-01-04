#include "VulkanRHIBuffer.h"

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHIBuffer::VulkanRHIBuffer(
			const Description& desc,
			VulkanRHIDevice::Ptr device,
			VkDeviceMemory memory,
			VkBuffer buffer
		)
			: RHIBuffer(desc), m_Device(device), m_Memory(memory), m_Buffer(buffer)
		{
		}

		VulkanRHIBuffer::~VulkanRHIBuffer()
		{
			if (m_Buffer != VK_NULL_HANDLE)
			{
				vkDestroyBuffer(m_Device->GetHandle(), m_Buffer, nullptr);
				m_Buffer = VK_NULL_HANDLE;
			}

			if (m_Memory != VK_NULL_HANDLE)
			{
				vkFreeMemory(m_Device->GetHandle(), m_Memory, nullptr);
				m_Memory = VK_NULL_HANDLE;
			}
		}

		void VulkanRHIBuffer::CopyData(int elementIndex, void* data, uint32_t dataSize)
		{
			assert(m_Description.UsageFlag != RHIBuffer::UsageFlag::GpuReadOnly);

			uint8_t* mappedData = nullptr;
			VULKAN_CALL_CHECK(vkMapMemory(
				m_Device->GetHandle(),
				m_Memory,
				0,
				VK_WHOLE_SIZE,
				0,
				reinterpret_cast<void**>(&mappedData)));

			memcpy(&mappedData[elementIndex * m_Description.ElementSize], data, dataSize);

			vkUnmapMemory(m_Device->GetHandle(), m_Memory);
		}

		RenderNativeObject VulkanRHIBuffer::GetNativeObject()
		{
			return RenderNativeObject(m_Buffer);
		}

		VkBuffer VulkanRHIBuffer::GetHandle() const
		{
			return m_Buffer;
		}

		uint32_t VulkanRHIBuffer::GetSize() const
		{
			return m_Description.Count * m_Description.ElementSize;
		}
	}
}