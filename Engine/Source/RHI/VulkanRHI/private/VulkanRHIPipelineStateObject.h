#pragma once

#include <VulkanRHIDevice.h>
#include <RHIPipelineStateObject.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHIPipelineStateObject final : public RHIPipelineStateObject
		{
		public:
			using Ptr = RefCountPtr<VulkanRHIPipelineStateObject>;

		public:
			VulkanRHIPipelineStateObject() = delete;
			VulkanRHIPipelineStateObject(const Description& desc, VulkanRHIDevice::Ptr device, VkPipeline pipeline);

			~VulkanRHIPipelineStateObject();

		public:
			virtual RenderNativeObject GetNativeObject() override;
			VkPipeline GetHandle() const;

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;

			VkPipeline m_NativePSO = VK_NULL_HANDLE;
		};
	}
}