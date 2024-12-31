#pragma once

#include <RHIDevice.h>
#include <VulkanRHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIDevice final : public RHIDevice
		{
		public:
			using Ptr = RefCountPtr<VulkanRHIDevice>;

		public:
			VulkanRHIDevice() = delete;
			explicit VulkanRHIDevice(VulkanRHIFactory::Ptr factory);
			~VulkanRHIDevice();

		public:
			virtual RenderNativeObject GetNativeObject() override;
			VkDevice GetHandle() const;
			VkPhysicalDevice GetPhysicalDevice() const;
			VkQueue GetMainQueue() const;
			uint32_t GetMainQueueFamilyIndex() const;

			VkDeviceMemory AllocateDeviceMemory(
				VkBuffer buffer,
				VkMemoryPropertyFlags properties
			);

			VkDeviceMemory AllocateDeviceMemory(
				VkImage image,
				VkMemoryPropertyFlags properties
			);

		private:
			void PickPhysicalDevice(VkInstance instance);
			void PickMainQueueFamily();
			void CreateLogicalDevice(VkInstance instance);

			uint32_t FindMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) const;

		private:
			VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
			VkDevice m_NativeHandle = VK_NULL_HANDLE;

			uint32_t m_MainQueueFamilyIndex = 0;
			VkQueue m_MainQueue = VK_NULL_HANDLE;
		};
	}
}