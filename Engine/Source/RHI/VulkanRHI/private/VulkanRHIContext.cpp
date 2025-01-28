#include <Windows.h>
#include <dxcapi.h>

#include <array>
#include <Debug/Assertions.h>
#include <Geometry.h>
#include <Debug/Console.h>
#include <Debug/Console.h>
#include <FileSystem.h>
#include <Vector.h>
#include <RHICommon.h>
#include <VulkanRHIBuffer.h>
#include <VulkanRHIFactory.h>
#include <VulkanRHIDevice.h>
#include <VulkanRHISwapChain.h>
#include <VulkanRHIFence.h>
#include <VulkanRHICommandQueue.h>
#include <VulkanRHICommandAllocator.h>
#include <VulkanRHICommandList.h>
#include <VulkanRHITexture.h>
#include <VulkanRHIContext.h>
#include <VulkanRHITechnique.h>
#include <VulkanRHIPipelineStateObject.h>
#include <VulkanShaderCompiler.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		static VkBufferUsageFlags ConvertToVkBufferUsage(RHIBuffer::UsageFlag usage)
		{
			switch (usage)
			{
			case RHIBuffer::UsageFlag::ConstantBuffer:
				return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			case RHIBuffer::UsageFlag::CpuWrite:
				return VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			case RHIBuffer::UsageFlag::GpuReadOnly:
				return VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return 0;
			}
		}

		static VkMemoryPropertyFlags ChooseMemoryProperties(RHIBuffer::UsageFlag usage)
		{
			switch (usage)
			{
			case RHIBuffer::UsageFlag::ConstantBuffer:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case RHIBuffer::UsageFlag::CpuWrite:
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case RHIBuffer::UsageFlag::GpuReadOnly:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return 0;
			}
		}

		static VkImageUsageFlags ConvertToVkImageUsage(RHITexture::UsageFlags::Flag usage)
		{
			VkImageUsageFlags vulkanUsage = 0;

			if (usage & RHITexture::UsageFlags::ShaderResource)
			{
				vulkanUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
			}

			if (usage & RHITexture::UsageFlags::RenderTarget)
			{
				vulkanUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			}

			if (usage & RHITexture::UsageFlags::DepthStencil)
			{
				vulkanUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			}

			return vulkanUsage;
		}

		VulkanRHIContext::VulkanRHIContext()
		{
			m_ShaderCompiler = std::make_unique<VulkanShaderCompiler>();

			m_Factory = new VulkanRHIFactory();
			m_Device = new VulkanRHIDevice(m_Factory);

			m_SwapChain = new VulkanRHISwapChain(m_Device, m_Factory);
			m_SwapChain->Resize(m_Device, 1.f, 1.f);

			m_Fence = new VulkanRHIFence(m_Device);

			m_CommandQueue = new VulkanRHICommandQueue(m_Device->GetMainQueue());
			m_CommandList = new VulkanRHICommandList(m_Device);

			RecreateTransferStagingBuffer(1024 * 1024);
			m_TransferCommandList = new VulkanRHICommandList(m_Device);
		}

		VulkanRHIContext::~VulkanRHIContext()
		{
		}

		RHITexture::Ptr VulkanRHIContext::CreateTexture(const RHITexture::Description& description)
		{
			VkImageType imageType;
			VkImageViewType viewType;
			switch (description.Dimension)
			{
			case RHITexture::Dimensions::One:
				imageType = VK_IMAGE_TYPE_1D;
				viewType = VK_IMAGE_VIEW_TYPE_1D;
				break;

			case RHITexture::Dimensions::Two:
				imageType = VK_IMAGE_TYPE_2D;
				viewType = VK_IMAGE_VIEW_TYPE_2D;
				break;

			case RHITexture::Dimensions::Three:
				imageType = VK_IMAGE_TYPE_3D;
				viewType = VK_IMAGE_VIEW_TYPE_3D;
				break;

			default:
				ASSERT_NOT_IMPLEMENTED;
			}

			/* Create image */
			const VkImageCreateInfo imageInfo =
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = imageType,
				.format = ConvertToVkFormat(description.Format),

				.extent =
				{
					.width = static_cast<uint32_t>(description.Width),
					.height = static_cast<uint32_t>(description.Height),
					.depth = 1,
				},

				.mipLevels = description.MipLevels,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = ConvertToVkImageUsage(description.Flags) | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			};

			VkImage vulkanImage = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkCreateImage(m_Device->GetHandle(), &imageInfo, nullptr, &vulkanImage));

			/* Allocate memory */
			VkDeviceMemory vulkanMemory = m_Device->AllocateDeviceMemory(vulkanImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			/* Create image view */
			VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_NONE;
			if (IsDepthStencilVkFormat(imageInfo.format))
			{
				aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			else if (IsDepthContainingVkFormat(imageInfo.format))
			{
				aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
			}
			else
			{
				aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
			}

			const VkImageViewCreateInfo viewInfo =
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = vulkanImage,
				.viewType = viewType,
				.format = imageInfo.format,

				.components =
				{
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY,
					.a = VK_COMPONENT_SWIZZLE_IDENTITY,
				},

				.subresourceRange =
				{
					.aspectMask = aspectMask,
					.baseMipLevel = 0,
					.levelCount = description.MipLevels,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			};

			VkImageView vulkanView = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkCreateImageView(m_Device->GetHandle(), &viewInfo, nullptr, &vulkanView));

			return VulkanRHITexture::Ptr(new VulkanRHITexture(description, m_Device, vulkanMemory, vulkanImage, vulkanView));
		}

		RHIBuffer::Ptr VulkanRHIContext::CreateBuffer(RHIBuffer::Description&& description)
		{
			if (description.UsageFlag == RHIBuffer::UsageFlag::ConstantBuffer)
			{
				// Should be multiple of 256
				description.ElementSize = (description.ElementSize + 255) & ~255;
			}

			const VkBufferCreateInfo bufferInfo =
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.size = description.Count * description.ElementSize,
				.usage = ConvertToVkBufferUsage(description.UsageFlag),
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
			};

			VkBuffer vulkanBuffer = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkCreateBuffer(m_Device->GetHandle(), &bufferInfo, nullptr, &vulkanBuffer));

			VkDeviceMemory vulkanMemory = m_Device->AllocateDeviceMemory(vulkanBuffer,
				ChooseMemoryProperties(description.UsageFlag));

			VulkanRHIBuffer::Ptr buffer = VulkanRHIBuffer::Ptr(new VulkanRHIBuffer(description,
				m_Device, vulkanMemory, vulkanBuffer));

			if (description.UsageFlag == RHIBuffer::UsageFlag::GpuReadOnly)
 			{
 				assert(description.initData != nullptr);

				uint32_t newSize = buffer->GetSize();
				RecreateTransferStagingBuffer(newSize);

				m_TransferCommandList->Reset();

				m_TransferStagingBuffer->CopyData(0, description.initData, description.Count * description.ElementSize);
				m_TransferCommandList->CopyBufferToBuffer(m_TransferStagingBuffer, buffer, newSize);

				m_TransferCommandList->Close();
				m_CommandQueue->ExecuteCommandLists({m_TransferCommandList});

				m_Fence->Sync(m_CommandQueue);
			}

			return buffer;
		}

		RHITechnique::Ptr VulkanRHIContext::CreateTechnique(
			const RHITechnique::ShaderInfo& shaderInfo,
			const RHITechnique::InputLayout& inputLayout,
			const RHITechnique::RootSignature& rootSignature)
		{
			/* Shaders */
			VulkanRHITechnique::ShaderModuleList shaderModules;
			shaderModules.reserve(shaderInfo.size());

			for (const RHITechnique::ShaderInfoDescription& shaderDesc : shaderInfo)
			{
				std::vector<uint32_t> spirv = m_ShaderCompiler->CompileFromFile(shaderDesc);

				const VkShaderModuleCreateInfo shaderModuleInfo =
				{
					.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.codeSize = spirv.size() * sizeof(spirv[0]),
					.pCode = spirv.data(),
				};

				VkShaderModule vulkanShaderModule = VK_NULL_HANDLE;
				VULKAN_CALL_CHECK(vkCreateShaderModule(m_Device->GetHandle(),
					&shaderModuleInfo, nullptr, &vulkanShaderModule));

				shaderModules[ConvertToVkShaderStage(shaderDesc.Type)] = vulkanShaderModule;
			}

			return VulkanRHITechnique::Ptr(new VulkanRHITechnique(m_Device, shaderInfo, inputLayout, rootSignature, std::move(shaderModules)));
		}

		RHIPipelineStateObject::Ptr VulkanRHIContext::CreatePSO(const RHIPipelineStateObject::Description& description)
		{
			VulkanRHITechnique* technique = reinterpret_cast<VulkanRHITechnique*>(description.Technique.Get());

			/* Shader Stages */
			const VulkanRHITechnique::ShaderModuleList& shaderModules = technique->GetShaderBlobs();

			std::vector<VkPipelineShaderStageCreateInfo> stageInfos;
			stageInfos.reserve(shaderModules.size());

			for (uint32_t i = 0; i < shaderModules.size(); ++i)
			{
				const RHITechnique::ShaderInfoDescription& shaderDesc = technique->GetShaderInfo()[i];
				VkShaderStageFlagBits shaderStageFlag = ConvertToVkShaderStage(shaderDesc.Type);

				stageInfos.push_back({
					.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.stage = shaderStageFlag,
					.module = shaderModules.at(shaderStageFlag),
					.pName = shaderDesc.EntryPoint.c_str(),
					.pSpecializationInfo = nullptr,
				});
			}

			/* Vertex Input State */
			VkPipelineVertexInputStateCreateInfo vertexInputState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.vertexBindingDescriptionCount = 0,
				.pVertexBindingDescriptions = nullptr,
				.vertexAttributeDescriptionCount = 0,
				.pVertexAttributeDescriptions = nullptr,
			};

			if (!technique->GetInputLayout().empty())
			{
				const VulkanRHITechnique::InputBindingList& bindings = technique->GetInputBindings();
				const VulkanRHITechnique::InputAttributeList& attribute = technique->GetInputAttributes();

				vertexInputState.vertexBindingDescriptionCount = bindings.size();
				vertexInputState.pVertexBindingDescriptions = bindings.data();
				vertexInputState.vertexAttributeDescriptionCount = attribute.size();
				vertexInputState.pVertexAttributeDescriptions = attribute.data();
			}

			/* Input Assembly State */
			const VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // Actually uses dynamic state
				.primitiveRestartEnable = VK_FALSE,
			};

			/* Viewport State (actually uses dynamic state, but still need to specify) */
			const VkViewport viewport =
			{
				.x = 0.0f,
				.y = 0.0f,
				.width = 0.0f,
				.height = 0.0f,
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};

			const VkRect2D scissor =
			{
				.offset = {0, 0},
				.extent = {0, 0},
			};

			const VkPipelineViewportStateCreateInfo viewportState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.viewportCount = 1,
				.pViewports = &viewport,
				.scissorCount = 1,
				.pScissors = &scissor,
			};

			/* Rasterization State */
			assert(description.RasterState.DepthClipEnable); // Not supported in Vulkan by default

			const VkPipelineRasterizationStateCreateInfo rasterizationState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = ConvertToVkPolygonMode(description.PrimitiveTopology),
				.cullMode = static_cast<VkCullModeFlags>(ConvertToVkCullMode(description.RasterState.cullMode)),
				.frontFace = description.RasterState.FrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE,
				.depthBiasEnable = VK_FALSE,
				.depthBiasConstantFactor = static_cast<float>(description.RasterState.DepthBias),
				.depthBiasClamp = description.RasterState.DepthBiasClamp,
				.depthBiasSlopeFactor = description.RasterState.SlopeScaledDepthBias,
				.lineWidth = 1.0f,
			};

			/* Multisampling State */
			assert(!description.RasterState.MultisampleEnable); // Not yet implemented

			const VkPipelineMultisampleStateCreateInfo multisampleState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
				.sampleShadingEnable = VK_FALSE,
				.minSampleShading = 1.0f,
				.pSampleMask = nullptr,
				.alphaToCoverageEnable = VK_FALSE,
				.alphaToOneEnable = VK_FALSE,
			};

			/* Depth Stencil State */
			const VkPipelineDepthStencilStateCreateInfo depthStencilState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.depthTestEnable = description.DepthStencilState.DepthEnable,
				.depthWriteEnable = description.DepthStencilState.DepthEnable,
				.depthCompareOp = ConvertToVkCompareOp(description.DepthStencilState.DepthFunc),
				.depthBoundsTestEnable = VK_FALSE,
				.stencilTestEnable = description.DepthStencilState.StencilEnable,

				.front =
				{
					.failOp = ConvertToVkStencilOp(description.DepthStencilState.FrontFace.StencilFailOp),
					.passOp = ConvertToVkStencilOp(description.DepthStencilState.FrontFace.StencilPassOp),
					.depthFailOp = ConvertToVkStencilOp(description.DepthStencilState.FrontFace.StencilDepthFailOp),
					.compareOp = ConvertToVkCompareOp(description.DepthStencilState.FrontFace.StencilFunc),
					.compareMask = description.DepthStencilState.StencilReadMask,
					.writeMask = description.DepthStencilState.StencilWriteMask,
					.reference = 0xff,
				},

				.back =
				{
					.failOp = ConvertToVkStencilOp(description.DepthStencilState.BackFace.StencilFailOp),
					.passOp = ConvertToVkStencilOp(description.DepthStencilState.BackFace.StencilPassOp),
					.depthFailOp = ConvertToVkStencilOp(description.DepthStencilState.BackFace.StencilDepthFailOp),
					.compareOp = ConvertToVkCompareOp(description.DepthStencilState.BackFace.StencilFunc),
					.compareMask = description.DepthStencilState.StencilReadMask,
					.writeMask = description.DepthStencilState.StencilWriteMask,
					.reference = 0xff,
				},

				.minDepthBounds = 0.0f,
				.maxDepthBounds = 1.0f,
			};

			/* Color Blending */
			std::vector<VkPipelineColorBlendAttachmentState> attachmentBlendState(description.NumRenderTargets);
			for (uint32_t i = 0; i < description.NumRenderTargets; ++i)
			{
				const RenderTargetBlendDescription& rtBlendDescription = description.BlendState.RenderTarget[i];

				attachmentBlendState[i] =
				{
					.blendEnable = rtBlendDescription.BlendEnable,
					.srcColorBlendFactor = ConvertToVkBlendFactor(rtBlendDescription.SrcBlend),
					.dstColorBlendFactor = ConvertToVkBlendFactor(rtBlendDescription.DestBlend),
					.colorBlendOp = ConvertToVkBlendOp(rtBlendDescription.BlendOp),
					.srcAlphaBlendFactor = ConvertToVkBlendFactor(rtBlendDescription.SrcBlendAlpha),
					.dstAlphaBlendFactor = ConvertToVkBlendFactor(rtBlendDescription.DestBlendAlpha),
					.alphaBlendOp = ConvertToVkBlendOp(rtBlendDescription.BlendOpAlpha),
					.colorWriteMask = rtBlendDescription.RenderTargetWriteMask,
				};
			}

			const VkPipelineColorBlendStateCreateInfo blendState =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.logicOpEnable = description.BlendState.RenderTarget[0].LogicOpEnable,
				.logicOp = ConvertToVkLogicOp(description.BlendState.RenderTarget[0].LogicOp),
				.attachmentCount = static_cast<uint32_t>(attachmentBlendState.size()),
				.pAttachments = attachmentBlendState.data(),
				.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
			};

			/* Pipeline Rendering Info */
			std::vector<VkFormat> colorFormats;
			colorFormats.reserve(description.NumRenderTargets);

			for (uint32_t i = 0; i < description.NumRenderTargets; ++i)
			{
				colorFormats.push_back(ConvertToVkFormat(description.RTVFormats[i]));
			}

			VkFormat depthStencilFormat = ConvertToVkFormat(description.DSVFormat);
			bool depthStencilUsed = description.DSVFormat != ResourceFormat::UNKNOWN;
			bool stencilUsed = depthStencilUsed && IsDepthStencilVkFormat(depthStencilFormat);

			const VkPipelineRenderingCreateInfo pipelineRenderingInfo =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
				.pNext = nullptr,
				.viewMask = 0,
				.colorAttachmentCount = static_cast<uint32_t>(colorFormats.size()),
				.pColorAttachmentFormats = colorFormats.data(),
				.depthAttachmentFormat = depthStencilUsed ? depthStencilFormat : VK_FORMAT_UNDEFINED,
				.stencilAttachmentFormat = stencilUsed ? depthStencilFormat : VK_FORMAT_UNDEFINED,
			};

			/* Dynamic States */
			static constexpr std::array DYNAMIC_STATES =
			{
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
				VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
			};

			const VkPipelineDynamicStateCreateInfo dynamicStateInfo =
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.dynamicStateCount = DYNAMIC_STATES.size(),
				.pDynamicStates = DYNAMIC_STATES.data(),
			};

			/* Create Vulkan Pipeline */
			const VkGraphicsPipelineCreateInfo pipelineInfo =
			{
				.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.pNext = &pipelineRenderingInfo,
				.flags = 0,
				.stageCount = static_cast<uint32_t>(stageInfos.size()),
				.pStages = stageInfos.data(),
				.pVertexInputState = &vertexInputState,
				.pInputAssemblyState = &inputAssemblyState,
				.pTessellationState = nullptr,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizationState,
				.pMultisampleState = &multisampleState,
				.pDepthStencilState = &depthStencilState,
				.pColorBlendState = &blendState,
				.pDynamicState = &dynamicStateInfo,
				.layout = technique->GetPipelineLayout(),
				.renderPass = VK_NULL_HANDLE,
				.subpass = 0,
				.basePipelineHandle = VK_NULL_HANDLE,
				.basePipelineIndex = -1,
			};

			VkPipeline vulkanPipeline = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkCreateGraphicsPipelines(m_Device->GetHandle(), VK_NULL_HANDLE, 1,
				&pipelineInfo, nullptr, &vulkanPipeline));

			return VulkanRHIPipelineStateObject::Ptr(new VulkanRHIPipelineStateObject(description, m_Device, vulkanPipeline));
		}

		RHIMesh::Ptr VulkanRHIContext::CreateMesh(const RHIMesh::VertexBufferDescription& vertexDesc, const RHIMesh::IndexBufferDescription& indexDesc)
		{
			assert(vertexDesc.initData);
			assert(indexDesc.initData);

			RHIBuffer::Ptr vertexBuffer = CreateBuffer(
				{
					.Count = vertexDesc.Count,
					.ElementSize = vertexDesc.ElementSize,
					.UsageFlag = HAL::RHIBuffer::UsageFlag::GpuReadOnly,
					.initData = vertexDesc.initData
				}
			);

			RHIBuffer::Ptr indexBuffer = CreateBuffer(
				{
					.Count = indexDesc.Count,
					.ElementSize = GetFormatSize(indexDesc.Format),
					.UsageFlag = HAL::RHIBuffer::UsageFlag::GpuReadOnly,
					.initData = indexDesc.initData
				}
			);

			return RHIMesh::Ptr(new RHIMesh(vertexBuffer, indexBuffer, indexDesc.Format));
		}

		RHIDevice::Ptr VulkanRHIContext::GetDevice() const
		{
			return m_Device;
		}

		RHIFactory::Ptr VulkanRHIContext::GetFactory() const
		{
			return m_Factory;
		}

		RHISwapChain::Ptr VulkanRHIContext::GetSwapChain() const
		{
			return m_SwapChain;
		}

		RHIFence::Ptr VulkanRHIContext::GetFence() const
		{
			return m_Fence;
		}

		RHICommandQueue::Ptr VulkanRHIContext::GetCommandQueue() const
		{
			return m_CommandQueue;
		}

		RHICommandList::Ptr VulkanRHIContext::GetCommandList() const
		{
			return m_CommandList;
		}

		void VulkanRHIContext::RecreateTransferStagingBuffer(uint32_t size)
		{
			if (m_TransferStagingBuffer != nullptr)
			{
				if (size < static_cast<VulkanRHIBuffer*>(m_TransferStagingBuffer.Get())->GetSize())
				{
					return;
				}

				m_TransferStagingBuffer.Reset();
			}

			m_TransferStagingBuffer = CreateBuffer({
				.Count = size,
				.ElementSize = 1,
				.UsageFlag = RHIBuffer::UsageFlag::CpuWrite,
				.initData = nullptr,
			});
		}
	}
}
