#include <VulkanRHIFence.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHIFence::VulkanRHIFence(VulkanRHIDevice::Ptr device)
			: m_Device(device)
		{
			//const VkSemaphoreTypeCreateInfo semaphoreTypeInfo =
			//{
			//	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
			//	.pNext = nullptr,
			//	.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
			//	.initialValue = 0,
			//};

			//const VkSemaphoreCreateInfo semaphoreInfo =
			//{
			//	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			//	.pNext = &semaphoreTypeInfo,
			//	.flags = 0,
			//};

			//VULKAN_CALL_CHECK(vkCreateSemaphore(m_Device->GetHandle(), &semaphoreInfo, nullptr, &m_NativeHandle));
		}

		//VulkanRHIFence::~VulkanRHIFence()
		//{
		//}

		void VulkanRHIFence::Sync(RHICommandQueue::Ptr commandQueue)
		{
			VULKAN_CALL_CHECK(vkQueueWaitIdle(commandQueue->GetNativeObject()));

			//++m_CurrentFence;

			////vkSetSemaphoreCounterValue()
			//const VkSemaphoreSignalInfo signalInfo =
			//{
			//	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			//	.pNext = nullptr,
			//	.semaphore = m_NativeHandle,
			//	.value = m_CurrentFence,
			//};

			////HRESULT hr = d3d12CommandQueue->Signal(m_NativeFence.Get(), m_CurrentFence);
			////assert(SUCCEEDED(hr));

			////if (m_NativeFence->GetCompletedValue() < m_CurrentFence)
			////{
			////	HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);
			////	assert(eventHandle);

			////	hr = m_NativeFence->SetEventOnCompletion(m_CurrentFence, eventHandle);
			////	assert(SUCCEEDED(hr));

			////	WaitForSingleObject(eventHandle, INFINITE);
			////	CloseHandle(eventHandle);
			////}
		}

		RenderNativeObject VulkanRHIFence::GetNativeObject()
		{
			return RenderNativeObject(VK_NULL_HANDLE);
			//return RenderNativeObject(m_NativeHandle);
		}

		//VkSemaphore VulkanRHIFence::GetHandle() const
		//{
		//	return m_NativeHandle;
		//}
	}
}