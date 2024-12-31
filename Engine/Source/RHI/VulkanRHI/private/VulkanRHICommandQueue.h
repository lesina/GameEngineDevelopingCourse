#pragma once

#include <VulkanRHIDevice.h>
#include <RHICommandQueue.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHICommandQueue final : public RHICommandQueue
		{
		public:
			using Ptr = RefCountPtr<VulkanRHICommandQueue>;

		public:
			VulkanRHICommandQueue() = delete;
			VulkanRHICommandQueue(VkQueue queue);
			~VulkanRHICommandQueue() = default;

		public:
			virtual void ExecuteCommandLists(const std::vector<RHICommandList::Ptr>& cmdLists) override;

			virtual RenderNativeObject GetNativeObject() override;

		private:
			VkQueue m_NativeCommandQueue = VK_NULL_HANDLE;
		};
	}
}