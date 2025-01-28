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

		public:
			virtual void Sync(RHICommandQueue::Ptr commandQueue) override;

			virtual RenderNativeObject GetNativeObject() override;

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;
		};
	}
}