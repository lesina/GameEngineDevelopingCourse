#include <VulkanRHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHITexture::VulkanRHITexture(
			const RHITexture::Description& desc,
			VulkanRHIDevice::Ptr device,
			VkDeviceMemory memory,
			VkImage image,
			VkImageView view
		)
			: RHITexture(desc)
			, m_Device(device)
			, m_Memory(memory)
			, m_Image(image)
			, m_View(view)
		{
		}

		VulkanRHITexture::~VulkanRHITexture()
		{
			if (m_View != VK_NULL_HANDLE)
			{
				vkDestroyImageView(m_Device->GetHandle(), m_View, nullptr);
				m_View = VK_NULL_HANDLE;
			}

			// In case the texture is a backbuffer, the image handle is owned by the swapchain itself
			if (IsOwning() && m_Image != VK_NULL_HANDLE)
			{
				vkDestroyImage(m_Device->GetHandle(), m_Image, nullptr);
				m_Image = VK_NULL_HANDLE;
			}

			if (m_Memory != VK_NULL_HANDLE)
			{
				vkFreeMemory(m_Device->GetHandle(), m_Memory, nullptr);
				m_Memory = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHITexture::GetNativeObject()
		{
			return RenderNativeObject(m_Image);
		}

		VkImage VulkanRHITexture::GetHandle() const
		{
			return m_Image;
		}

		VkImageView VulkanRHITexture::GetView() const
		{
			return m_View;
		}

		VulkanRHITexture::State VulkanRHITexture::GetCurrentState() const
		{
			return m_State;
		}

		void VulkanRHITexture::SetCurrentState(State state)
		{
			m_State = state;
		}

		bool VulkanRHITexture::IsOwning() const
		{
			return m_Memory != VK_NULL_HANDLE;
		}
	}
}