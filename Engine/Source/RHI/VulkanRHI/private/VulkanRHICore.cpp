#pragma once

#include <VulkanRHICore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VkFormat ConvertToVkFormat(ResourceFormat resourceFormat)
		{
			switch (resourceFormat)
			{
			case ResourceFormat::RGBA8_UNORM:
				return VK_FORMAT_R8G8B8A8_UNORM;
			case ResourceFormat::RGB32_FLOAT:
				return VK_FORMAT_R32G32B32_SFLOAT;
			case ResourceFormat::D24S8:
				return VK_FORMAT_D24_UNORM_S8_UINT;
			case ResourceFormat::R16_UNORM:
				return VK_FORMAT_R16_UNORM;
			case ResourceFormat::R16_UINT:
				return VK_FORMAT_R16_UINT;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_FORMAT_UNDEFINED;
			}
		}

		ResourceFormat ConvertToResourceFormat(VkFormat resourceFormat)
		{
			switch (resourceFormat)
			{
			case VK_FORMAT_R8G8B8A8_UNORM:
				return ResourceFormat::RGBA8_UNORM;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return ResourceFormat::RGBA8_UNORM;
			}
		}

		bool IsDepthContainingVkFormat(VkFormat format)
		{
			return format == VK_FORMAT_D16_UNORM ||
				format == VK_FORMAT_D32_SFLOAT ||
				format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		bool IsDepthStencilVkFormat(VkFormat format)
		{
			return format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		VkImageAspectFlags ConvertToImageAspectFlags(ResourceFormat format)
		{
			const VkFormat vkFormat = ConvertToVkFormat(format);

			if (IsDepthStencilVkFormat(vkFormat))
			{
				return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			else if (IsDepthContainingVkFormat(vkFormat))
			{
				return VK_IMAGE_ASPECT_DEPTH_BIT;
			}

			return VK_IMAGE_ASPECT_COLOR_BIT;
		}

		VkShaderStageFlagBits ConvertToVkShaderStage(RHITechnique::ShaderInfoDescription::ShaderType shaderType)
		{
			switch (shaderType)
			{
			case RHITechnique::ShaderInfoDescription::ShaderType::VertexShader:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case RHITechnique::ShaderInfoDescription::ShaderType::PixelShader:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_SHADER_STAGE_ALL;
			}
		}

		VkVertexInputRate ConvertToVkVertexInputRate(RHITechnique::InputLayoutDescription::Classification classification)
		{
			switch (classification)
			{
			case RHITechnique::InputLayoutDescription::Classification::PerVertex:
				return VK_VERTEX_INPUT_RATE_VERTEX;
			case RHITechnique::InputLayoutDescription::Classification::PerInstance:
				return VK_VERTEX_INPUT_RATE_INSTANCE;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_VERTEX_INPUT_RATE_VERTEX;
			}
		}

		VkPolygonMode ConvertToVkPolygonMode(PrimitiveTopologyType topologyType)
		{
			switch (topologyType)
			{
			case PrimitiveTopologyType::Triangle:
				return VK_POLYGON_MODE_FILL;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_POLYGON_MODE_FILL;
			}
		}

		VkPrimitiveTopology ConvertToVkPrimitiveTopology(PrimitiveTopology topology)
		{
			switch (topology)
			{
			case PrimitiveTopology::TriangleList:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			}
		}

		VkCullModeFlagBits ConvertToVkCullMode(CullMode cullMode)
		{
			switch (cullMode)
			{
			case CullMode::None:
				return VK_CULL_MODE_NONE;
			case CullMode::Front:
				return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:
				return VK_CULL_MODE_BACK_BIT;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_CULL_MODE_NONE;
			}
		}

		VkCompareOp ConvertToVkCompareOp(ComparisonFunc compFunc)
		{
			switch (compFunc)
			{
			case ComparisonFunc::None:
			case ComparisonFunc::Never:
				return VK_COMPARE_OP_NEVER;
			case ComparisonFunc::Less:
				return VK_COMPARE_OP_LESS;
			case ComparisonFunc::Equal:
				return VK_COMPARE_OP_EQUAL;
			case ComparisonFunc::LessEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case ComparisonFunc::Greater:
				return VK_COMPARE_OP_GREATER;
			case ComparisonFunc::NotEqual:
				return VK_COMPARE_OP_NOT_EQUAL;
			case ComparisonFunc::GreaterEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case ComparisonFunc::Always:
				return VK_COMPARE_OP_ALWAYS;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_COMPARE_OP_NEVER;
			}
		}

		VkStencilOp ConvertToVkStencilOp(StencilOp stencilOp)
		{
			switch (stencilOp)
			{
			case StencilOp::Keep:
				return VK_STENCIL_OP_KEEP;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_STENCIL_OP_KEEP;
			}
		}

		VkBlendFactor ConvertToVkBlendFactor(Blend blend)
		{
			switch (blend)
			{
			case Blend::One:
				return VK_BLEND_FACTOR_ONE;
			case Blend::Zero:
				return VK_BLEND_FACTOR_ZERO;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_BLEND_FACTOR_ONE;
			}
		}

		VkBlendOp ConvertToVkBlendOp(BlendOperation blendOp)
		{
			switch (blendOp)
			{
			case BlendOperation::Add:
				return VK_BLEND_OP_ADD;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_BLEND_OP_ADD;
			}
		}

		VkLogicOp ConvertToVkLogicOp(LogicOperation logicOp)
		{
			switch (logicOp)
			{
			case LogicOperation::Noop:
				return VK_LOGIC_OP_NO_OP;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_LOGIC_OP_NO_OP;
			}
		}

		VkIndexType ConvertToVkIndexType(ResourceFormat resourceFormat)
		{
			switch (resourceFormat)
			{
			case ResourceFormat::R16_UINT:
				return VK_INDEX_TYPE_UINT16;
			case ResourceFormat::R32_UINT:
				return VK_INDEX_TYPE_UINT32;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return VK_INDEX_TYPE_UINT16;
			}
		}
	}
}