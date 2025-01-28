#include <VulkanRHICommandQueue.h>
#include <VulkanRHICommandList.h>

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
			std::vector<VkCommandBufferSubmitInfo> cmdSubmitInfos;
			cmdSubmitInfos.reserve(cmdLists.size());

			for (size_t index = 0; index < cmdLists.size(); ++index)
			{
				VulkanRHICommandList* cmdList = static_cast<VulkanRHICommandList*>(cmdLists[index].Get());
				if (cmdList->GetCommandsRecorded() == 0)
				{
					continue;
				}

				cmdSubmitInfos.push_back({
					.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
					.pNext = nullptr,
					.commandBuffer = (VkCommandBuffer)(cmdList->GetNativeObject()),
					.deviceMask = 0,
				});

				cmdSubmitInfos.back().commandBuffer = (VkCommandBuffer)(cmdList->GetNativeObject());
			}

			const VkSubmitInfo2 submitInfo =
			{
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
				.pNext = nullptr,
				.flags = 0,
				.waitSemaphoreInfoCount = 0,
				.pWaitSemaphoreInfos = nullptr,
				.commandBufferInfoCount = static_cast<uint32_t>(cmdSubmitInfos.size()),
				.pCommandBufferInfos = cmdSubmitInfos.data(),
				.signalSemaphoreInfoCount = 0,
				.pSignalSemaphoreInfos = nullptr,
			};

			VULKAN_CALL_CHECK(vkQueueSubmit2(m_NativeCommandQueue, 1, &submitInfo, VK_NULL_HANDLE));
		}

		RenderNativeObject VulkanRHICommandQueue::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandQueue);
		}
	}
}