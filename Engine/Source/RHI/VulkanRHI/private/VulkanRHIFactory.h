#pragma once

#include <VulkanRHICore.h>
#include <RHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIFactory final : public RHIFactory
		{
		public:
			using Ptr = RefCountPtr<VulkanRHIFactory>;

		public:
			VulkanRHIFactory();
			~VulkanRHIFactory();

		public:
			virtual RenderNativeObject GetNativeObject() override;
			VkInstance GetHandle() const;

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT severity,
				VkDebugUtilsMessageTypeFlagsEXT type,
				const VkDebugUtilsMessengerCallbackDataEXT* data,
				void* userData
			);

		private:
			VkInstance m_NativeHandle = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;
		};
	}
}