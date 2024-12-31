#pragma once

#include <VulkanRHIDevice.h>
#include <RHICommandAllocator.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHICommandAllocator final : public RHICommandAllocator
		{
		public:
			using Ptr = RefCountPtr<VulkanRHICommandAllocator>;

		public:
			VulkanRHICommandAllocator() = delete;
			VulkanRHICommandAllocator(VulkanRHIDevice::Ptr device);
			~VulkanRHICommandAllocator();

		public:
			virtual void Reset() override;

			virtual RenderNativeObject GetNativeObject() override;
			VkCommandPool GetHandle() const;

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;
			VkCommandPool m_NativeCommandPool = VK_NULL_HANDLE;
		};
	}
}