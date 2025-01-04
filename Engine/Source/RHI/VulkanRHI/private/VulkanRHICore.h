#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <volk.h>

#include <Debug/Assertions.h>
#include <RHITechnique.h>
#include <RHICore.h>

#define VULKAN_CALL_CHECK(Call) \
	{ \
		VkResult result = Call; \
		assert(result == VK_SUCCESS); \
	}

namespace GameEngine
{
	namespace Render::HAL
	{
		VkFormat ConvertToVkFormat(ResourceFormat resourceFormat);
		ResourceFormat ConvertToResourceFormat(VkFormat resourceFormat);

		bool IsDepthContainingVkFormat(VkFormat format);
		bool IsDepthStencilVkFormat(VkFormat format);

		VkImageAspectFlags ConvertToImageAspectFlags(ResourceFormat format);

		VkShaderStageFlagBits ConvertToVkShaderStage(RHITechnique::ShaderInfoDescription::ShaderType shaderType);
		VkVertexInputRate ConvertToVkVertexInputRate(RHITechnique::InputLayoutDescription::Classification classification);
		VkPolygonMode ConvertToVkPolygonMode(PrimitiveTopologyType topologyType);
		VkPrimitiveTopology ConvertToVkPrimitiveTopology(PrimitiveTopology topology);
		VkCullModeFlagBits ConvertToVkCullMode(CullMode cullMode);
		VkCompareOp ConvertToVkCompareOp(ComparisonFunc compFunc);
		VkStencilOp ConvertToVkStencilOp(StencilOp stencilOp);
		VkBlendFactor ConvertToVkBlendFactor(Blend blend);
		VkBlendOp ConvertToVkBlendOp(BlendOperation blendOp);
		VkLogicOp ConvertToVkLogicOp(LogicOperation logicOp);
		VkIndexType ConvertToVkIndexType(ResourceFormat resourceFormat);
	}
}