#pragma once

#include <unordered_map>
#include <vector>
#include <VulkanRHIDevice.h>
#include <RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanRHITechnique final : public RHITechnique
		{
		public:
			using Ptr = RefCountPtr<VulkanRHITechnique>;
			using ShaderModuleList = std::unordered_map<VkShaderStageFlagBits, VkShaderModule>;
			using InputBindingList = std::vector<VkVertexInputBindingDescription>;
			using InputAttributeList = std::vector<VkVertexInputAttributeDescription>;

		public:
			VulkanRHITechnique() = delete;
			VulkanRHITechnique(
				VulkanRHIDevice::Ptr device,
				const ShaderInfo& shaderInfo,
				const InputLayout& inputLayout,
				const RootSignature& rootSignature,
				ShaderModuleList&& shaderBlobList
			);

			~VulkanRHITechnique();

		public:
			virtual RenderNativeObject GetNativeObject() override;

			const ShaderInfo& GetShaderInfo() const { return m_ShaderInfo; }
			const InputLayout& GetInputLayout() const { return m_InputLayout; }

			VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }
			VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

			const InputBindingList& GetInputBindings() const { return m_InputBindingList; }
			const InputAttributeList& GetInputAttributes() const { return m_InputAttributeList; }
			const ShaderModuleList& GetShaderBlobs() const { return m_ShaderModuleList; }

		private:
			void FillInputAssemblyInfo();
			void CreateDescriptorSetLayout();
			void CreatePipelineLayout();

		private:
			VulkanRHIDevice::Ptr m_Device = nullptr;

			VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
			VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

			ShaderModuleList m_ShaderModuleList;
			InputBindingList m_InputBindingList;
			InputAttributeList m_InputAttributeList;
		};
	}
}