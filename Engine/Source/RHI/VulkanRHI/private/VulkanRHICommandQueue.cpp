#include <VulkanRHICommandQueue.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHICommandQueue::VulkanRHICommandQueue(VkQueue queue)
			: m_NativeCommandQueue(queue)
		{
		}

		void VulkanRHICommandQueue::ExecuteCommandLists(const std::vector<RHICommandList::Ptr>& cmdLists)
		{
			std::vector<VkCommandBufferSubmitInfo> cmdSubmitInfos(cmdLists.size());

			for (size_t index = 0; index < cmdLists.size(); ++index)
			{
				cmdSubmitInfos[index] =
				{
					.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
					.pNext = nullptr,
					.commandBuffer = (VkCommandBuffer)(cmdLists[index]->GetNativeObject()),
					.deviceMask = 0,
				};

				cmdSubmitInfos[index].commandBuffer = (VkCommandBuffer)(cmdLists[index]->GetNativeObject());
			}

			const VkSubmitInfo2 submitInfo =
			{
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
				.pNext = nullptr,
				.flags = 0,
				.waitSemaphoreInfoCount = 0, // TODO
				.pWaitSemaphoreInfos = nullptr, // TODO
				.commandBufferInfoCount = static_cast<uint32_t>(cmdSubmitInfos.size()),
				.pCommandBufferInfos = cmdSubmitInfos.data(),
				.signalSemaphoreInfoCount = 0, // TODO
				.pSignalSemaphoreInfos = nullptr, // TODO
			};

			VULKAN_CALL_CHECK(vkQueueSubmit2(m_NativeCommandQueue, 1, &submitInfo, VK_NULL_HANDLE));
		}

		RenderNativeObject VulkanRHICommandQueue::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandQueue);
		}
	}
}