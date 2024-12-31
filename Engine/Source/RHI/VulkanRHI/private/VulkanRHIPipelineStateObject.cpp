#include <VulkanRHIPipelineStateObject.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHIPipelineStateObject::VulkanRHIPipelineStateObject(
			const Description& desc,
			VulkanRHIDevice::Ptr device,
			VkPipeline pipeline
		)
			: RHIPipelineStateObject(desc)
			, m_Device(device)
			, m_NativePSO(pipeline)
		{
		}

		VulkanRHIPipelineStateObject::~VulkanRHIPipelineStateObject()
		{
			if (m_NativePSO != VK_NULL_HANDLE)
			{
				vkDestroyPipeline(m_Device->GetHandle(), m_NativePSO, nullptr);
				m_NativePSO = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHIPipelineStateObject::GetNativeObject()
		{
			return RenderNativeObject(m_NativePSO);
		}

		VkPipeline VulkanRHIPipelineStateObject::GetHandle() const
		{
			return m_NativePSO;
		}
	}
}