#include <array>

#include <VulkanRHICore.h>
#include <VulkanRHISwapChain.h>
#include <VulkanRHITexture.h>
#include <VulkanRHICommandList.h>

#include <RenderCore.h>
#include <Window/IWindow.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHISwapChain::VulkanRHISwapChain(
			VulkanRHIDevice::Ptr device,
			VulkanRHIFactory::Ptr factory
		)
			: m_Device(device)
			, m_Factory(factory)
			, m_BackBufferFormat(ConvertToVkFormat(RHISettings::BackBuffer))
		{
			CreateSurface(factory->GetHandle());
			CreateSwapChain();

			const VkFenceCreateInfo fenceInfo =
			{
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.pNext = nullptr,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT,
			};

			VULKAN_CALL_CHECK(vkCreateFence(m_Device->GetHandle(), &fenceInfo, nullptr, &m_AcquireFence));

			AcquireNext();
		}

		VulkanRHISwapChain::~VulkanRHISwapChain()
		{
			DestroySwapChain();
			vkDestroySurfaceKHR(m_Factory->GetHandle(), m_Surface, nullptr);
			vkDestroyFence(m_Device->GetHandle(), m_AcquireFence, nullptr);
		}

		void VulkanRHISwapChain::Present()
		{
			VULKAN_CALL_CHECK(vkQueueWaitIdle(m_Device->GetMainQueue()));

			const uint32_t imageIndex = static_cast<uint32_t>(m_CurrentBackBufferIdx);

			const VkPresentInfoKHR present_info =
			{
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext = nullptr,
				.waitSemaphoreCount = 0U,
				.pWaitSemaphores = nullptr,
				.swapchainCount = 1,
				.pSwapchains = &m_SwapChain,
				.pImageIndices = &imageIndex,
				.pResults = nullptr,
			};

			const VkResult res = vkQueuePresentKHR(m_Device->GetMainQueue(), &present_info);
			if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
			{
				// TODO: Handle swapchain resize on suboptimal present
			}

			AcquireNext();
		}

		void VulkanRHISwapChain::MakeBackBufferPresentable(RHICommandList::Ptr commandList)
		{
			assert(commandList != nullptr);

			VulkanRHITexture* backBuffer = m_BackBuffer[m_CurrentBackBufferIdx].Get();
			VulkanRHICommandList* commandBuffer = reinterpret_cast<VulkanRHICommandList*>(commandList.Get());

			const VulkanRHITexture::State currentState = backBuffer->GetCurrentState();

			if (currentState.Layout != VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				commandBuffer->EndRenderPassIfBound();

				commandBuffer->ValidateTextureState(
					m_BackBuffer[m_CurrentBackBufferIdx],
					{
						.Stages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
						.Access = VK_ACCESS_2_NONE,
						.Layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
					}
				);
			}
		}

		RenderNativeObject VulkanRHISwapChain::GetNativeObject()
		{
			return RenderNativeObject(m_SwapChain);
		}

		VkSwapchainKHR VulkanRHISwapChain::GetHandle() const
		{
			return m_SwapChain;
		}

		void VulkanRHISwapChain::Resize(RHIDevice::Ptr /*device*/, uint32_t width, uint32_t height)
		{
			DestroySwapChain();
			CreateSwapChain();

			std::array<VkImage, RenderCore::g_FrameBufferCount> vkImages;

			uint32_t imageCount = 0;
			VULKAN_CALL_CHECK(vkGetSwapchainImagesKHR(m_Device->GetHandle(), m_SwapChain, &imageCount, nullptr));
			assert(imageCount == RenderCore::g_FrameBufferCount);

			VULKAN_CALL_CHECK(vkGetSwapchainImagesKHR(m_Device->GetHandle(), m_SwapChain, &imageCount, vkImages.data()));

			for (uint32_t i = 0; i < imageCount; ++i)
			{
				const VkImageViewCreateInfo viewInfo =
				{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.image = vkImages[i],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = m_BackBufferFormat,
					.components =
					{
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY,
					},
					.subresourceRange =
					{
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					},
				};

				VkImageView view = VK_NULL_HANDLE;
				VULKAN_CALL_CHECK(vkCreateImageView(m_Device->GetHandle(), &viewInfo, nullptr, &view));

				m_BackBuffer[i] = new VulkanRHITexture(
					{
						.Dimension = RHITexture::Dimensions::Two,
						.Width = width,
						.Height = height,
						.MipLevels = 1,
						.Format = ConvertToResourceFormat(m_BackBufferFormat),
						.Flags = RHITexture::UsageFlags::RenderTarget
					},
					m_Device,
					VK_NULL_HANDLE,
					vkImages[i],
					view);
			}

			AcquireNext();
		}

		RHITexture::Ptr VulkanRHISwapChain::GetCurrentBackBuffer()
		{
			return m_BackBuffer[m_CurrentBackBufferIdx];
		}

		void VulkanRHISwapChain::CreateSurface(VkInstance instance)
		{
			const VkWin32SurfaceCreateInfoKHR surfaceInfo =
			{
				.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
				.pNext = nullptr,
				.flags = 0,
				.hinstance = reinterpret_cast<HINSTANCE>(Core::g_MainWindowsApplication->GetInstanceHandle()),
				.hwnd = reinterpret_cast<HWND>(Core::g_MainWindowsApplication->GetWindowHandle()),
			};

			VULKAN_CALL_CHECK(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &m_Surface));
		}

		void VulkanRHISwapChain::CreateSwapChain()
		{
			const VkExtent2D extent =
			{
				.width = Core::g_MainWindowsApplication->GetWidth(),
				.height = Core::g_MainWindowsApplication->GetHeight(),
			};

			const VkSwapchainCreateInfoKHR swapChainInfo =
			{
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.pNext = nullptr,
				.flags = 0,
				.surface = m_Surface,
				.minImageCount = RenderCore::g_FrameBufferCount,
				.imageFormat = m_BackBufferFormat,
				.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
				.imageExtent = extent,
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = VK_PRESENT_MODE_FIFO_KHR,
				.clipped = VK_TRUE,
				.oldSwapchain = VK_NULL_HANDLE,
			};

			VULKAN_CALL_CHECK(vkCreateSwapchainKHR(m_Device->GetHandle(), &swapChainInfo, nullptr, &m_SwapChain));
		}

		void VulkanRHISwapChain::DestroySwapChain()
		{
			for (auto& backBuffer : m_BackBuffer)
			{
				backBuffer.Reset();
			}

			vkDestroySwapchainKHR(m_Device->GetHandle(), m_SwapChain, nullptr);
		}

		void VulkanRHISwapChain::AcquireNext()
		{
			vkResetFences(m_Device->GetHandle(), 1, &m_AcquireFence);

			uint32_t imageIdx = 0;
			const VkResult res = vkAcquireNextImageKHR(m_Device->GetHandle(), m_SwapChain, UINT64_MAX, VK_NULL_HANDLE, m_AcquireFence, &imageIdx);
			if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
			{
				// TODO: Handle swapchain resize on suboptimal acquire
			}

			m_CurrentBackBufferIdx = imageIdx;

			vkWaitForFences(m_Device->GetHandle(), 1, &m_AcquireFence, VK_TRUE, UINT64_MAX);
		}
	}
}