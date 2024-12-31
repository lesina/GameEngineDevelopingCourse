#pragma once

#include <VulkanRHIDevice.h>
#include <RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHITexture final : public RHITexture
		{
		public:
			using Ptr = RefCountPtr<VulkanRHITexture>;

			struct State
			{
				VkPipelineStageFlags2 Stages = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
				VkAccessFlags2 Access = VK_ACCESS_2_NONE;
				VkImageLayout Layout = VK_IMAGE_LAYOUT_UNDEFINED;
			};

		public:
			VulkanRHITexture() = delete;
			VulkanRHITexture(
				const RHITexture::Description& desc,
				VulkanRHIDevice::Ptr device,
				VkDeviceMemory memory,
				VkImage image,
				VkImageView view
			);

			~VulkanRHITexture() override;

		public:
			virtual RenderNativeObject GetNativeObject() override;

			VkImage GetHandle() const;
			VkImageView GetView() const;

			State GetCurrentState() const;
			void SetCurrentState(State state);

		private:
			bool IsOwning() const;

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;

			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			VkImage m_Image = VK_NULL_HANDLE;
			VkImageView m_View = VK_NULL_HANDLE;

			State m_State;
		};
	}
}