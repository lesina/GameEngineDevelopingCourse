#pragma once

#include <VulkanRHICommandQueue.h>
#include <VulkanRHIFactory.h>
#include <VulkanRHITexture.h>
#include <RHISwapChain.h>

#include <RenderCore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHISwapChain final : public RHISwapChain
		{
		public:
			using Ptr = RefCountPtr<VulkanRHISwapChain>;

		public:
			VulkanRHISwapChain() = delete;
			VulkanRHISwapChain(
				VulkanRHIDevice::Ptr device,
				VulkanRHIFactory::Ptr factory
			);
			~VulkanRHISwapChain();

		public:
			virtual void Present() override;
			virtual void MakeBackBufferPresentable(RHICommandList::Ptr commandList) override;

			virtual RenderNativeObject GetNativeObject() override;
			VkSwapchainKHR GetHandle() const;

		public:
			virtual void Resize(RHIDevice::Ptr device, uint32_t width, uint32_t height) override;
			virtual RHITexture::Ptr GetCurrentBackBuffer() override;

		private:
			void CreateSurface(VkInstance instance);
			void CreateSwapChain();

			void DestroySwapChain();

			void AcquireNext();

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;
			VulkanRHIFactory::Ptr m_Factory = nullptr;

			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
			VkFormat m_BackBufferFormat;
			VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

			VkFence m_AcquireFence = VK_NULL_HANDLE;

			VulkanRHITexture::Ptr m_BackBuffer[RenderCore::g_FrameBufferCount];
		};
	}
}