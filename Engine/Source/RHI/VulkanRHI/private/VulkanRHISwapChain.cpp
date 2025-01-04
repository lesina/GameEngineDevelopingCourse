#include <array>

#include <VulkanRHICore.h>
#include <VulkanRHISwapChain.h>
#include <VulkanRHITexture.h>

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
			: m_Device(device), m_Factory(factory), m_BackBufferFormat(ConvertToVkFormat(RHISettings::BackBuffer))
		{
			CreateSurface(factory->GetHandle());
			CreateSwapChain();
		}

		VulkanRHISwapChain::~VulkanRHISwapChain()
		{
			DestroySwapChain();
			vkDestroySurfaceKHR(m_Factory->GetHandle(), m_Surface, nullptr);
		}

		void VulkanRHISwapChain::Present()
		{
			// TODO
			//HRESULT hr = m_NativeSwapChain->Present(0, 0);
			//assert(SUCCEEDED(hr));

			const uint32_t imageIndex = static_cast<uint32_t>(m_CurrentBackBufferIdx);

			const VkPresentInfoKHR present_info =
			{
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext = nullptr,
				.waitSemaphoreCount = 0U,  // TODO
				.pWaitSemaphores = nullptr,  // TODO
				.swapchainCount = 1,
				.pSwapchains = &m_SwapChain,
				.pImageIndices = &imageIndex,
				.pResults = nullptr,
			};

			const VkResult res = vkQueuePresentKHR(m_Device->GetMainQueue(), &present_info);
			if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
				// TODO

				//valid = false;
				//WaitIdle();
			}

			// FIXME: actually get the real image idx
			m_CurrentBackBufferIdx = (m_CurrentBackBufferIdx + 1) % RenderCore::g_FrameBufferCount;
		}

		void VulkanRHISwapChain::MakeBackBufferPresentable(RHICommandList::Ptr commandList)
		{
			assert(commandList != nullptr);

			VulkanRHITexture* backBuffer = m_BackBuffer[m_CurrentBackBufferIdx].Get();
			//VulkanRHICommandList* backBuffer = reinterpret_cast<D3D12RHICommandList*>(commandList.Get());

			const VulkanRHITexture::State currentState = backBuffer->GetCurrentState();

			if (currentState.Layout != VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				// TODO

				//D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer->GetHandle(),
				//	backBuffer->GetCurrentState(), D3D12_RESOURCE_STATE_PRESENT);
				//d3d12CommandList->GetHandle()->ResourceBarrier(1, &ResBarrierTransition);

				//backBuffer->SetCurrentState(D3D12_RESOURCE_STATE_PRESENT);
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
			// TODO: Is actually needed?

			//VkSurfaceCapabilitiesKHR surfaceCapabilities;
			//VULKAN_CALL_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->GetPhysicalDevice(), m_Surface, &surfaceCapabilities));

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
	}
}