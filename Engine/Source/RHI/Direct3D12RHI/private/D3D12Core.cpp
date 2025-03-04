#pragma once

#include <D3D12Core.h>

namespace cqe
{
	namespace Render::RHI
	{
		DXGI_FORMAT ConvertToDXGIFormat(const ResourceFormat& resourceFormat)
		{
			switch (resourceFormat)
			{
			case ResourceFormat::RGBA8_UNORM:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case ResourceFormat::RGB32_FLOAT:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case ResourceFormat::RGBA32_FLOAT:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case ResourceFormat::RG32_FLOAT:
				return DXGI_FORMAT_R32G32_FLOAT;
			case ResourceFormat::D24S8:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case ResourceFormat::R16_UNORM:
				return DXGI_FORMAT_R16_UNORM;
			case ResourceFormat::R16_UINT:
				return DXGI_FORMAT_R16_UINT;
			case ResourceFormat::BC1_UNORM:
				return DXGI_FORMAT_BC1_UNORM;
			case ResourceFormat::BC1_UNORM_SRGB:
				return DXGI_FORMAT_BC1_UNORM_SRGB;
			case ResourceFormat::BC2_UNORM:
				return DXGI_FORMAT_BC2_UNORM;
			case ResourceFormat::BC2_UNORM_SRGB:
				return DXGI_FORMAT_BC2_UNORM_SRGB;
			case ResourceFormat::BC3_UNORM:
				return DXGI_FORMAT_BC3_UNORM;
			case ResourceFormat::BC3_UNORM_SRGB:
				return DXGI_FORMAT_BC3_UNORM_SRGB;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		ResourceFormat ConvertToResourceFormat(DXGI_FORMAT resourceFormat)
		{
			switch (resourceFormat)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
				return ResourceFormat::RGBA8_UNORM;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return ResourceFormat::RGBA8_UNORM;
			}
		}

		std::string GetShaderTarget(const Technique::ShaderInfoDescription::ShaderType& shaderType)
		{
			switch (shaderType)
			{
			case Technique::ShaderInfoDescription::ShaderType::VertexShader:
				return "vs_5_0";
			case Technique::ShaderInfoDescription::ShaderType::PixelShader:
				return "ps_5_0";
			default:
				ASSERT_NOT_IMPLEMENTED;
				return "undefined";
			}
		}

		D3D12_INPUT_CLASSIFICATION ConvertToD3D12InputClassification(const Technique::InputLayoutDescription::Classification& classification)
		{
			switch (classification)
			{
			case Technique::InputLayoutDescription::Classification::PerVertex:
				return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			case Technique::InputLayoutDescription::Classification::PerInstance:
				return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			}
		}

		D3D12_RASTERIZER_DESC ConvertToD3D12RasterState(const RasterizerDescription& rasterState)
		{
			/*
			psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);*/
			D3D12_RASTERIZER_DESC d3d12RasterState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			d3d12RasterState.FillMode = rasterState.fillMode == FillMode::Wireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
			d3d12RasterState.CullMode = rasterState.cullMode == CullMode::Back ? D3D12_CULL_MODE_BACK :
				rasterState.cullMode == CullMode::Front ? D3D12_CULL_MODE_FRONT : D3D12_CULL_MODE_NONE;
			d3d12RasterState.FrontCounterClockwise = rasterState.FrontCounterClockwise;
			d3d12RasterState.DepthBias = rasterState.DepthBias;
			d3d12RasterState.DepthBiasClamp = rasterState.DepthBiasClamp;
			d3d12RasterState.SlopeScaledDepthBias = rasterState.SlopeScaledDepthBias;
			d3d12RasterState.DepthClipEnable = rasterState.DepthClipEnable;
			d3d12RasterState.MultisampleEnable = rasterState.MultisampleEnable;
			return d3d12RasterState;
		}

		D3D12_BLEND ConvertToD3D12Blend(const Blend& blend)
		{
			switch (blend)
			{
			case Blend::One:
				return D3D12_BLEND_ONE;
			case Blend::Zero:
				return D3D12_BLEND_ZERO;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_BLEND_ONE;
			}
		}

		D3D12_BLEND_OP ConvertToD3D12BlendOp(const BlendOperation& blendOp)
		{
			switch (blendOp)
			{
			case BlendOperation::Add:
				return D3D12_BLEND_OP_ADD;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_BLEND_OP_ADD;
			}
		}

		D3D12_LOGIC_OP ConvertToD3D12LogicOp(const LogicOperation& logicOp)
		{
			switch (logicOp)
			{
			case LogicOperation::Noop:
				return D3D12_LOGIC_OP_NOOP;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_LOGIC_OP_NOOP;
			}
		}

		D3D12_BLEND_DESC ConvertToD3D12BlendState(const BlendDescription& blendState)
		{
			D3D12_BLEND_DESC d3d12BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

			d3d12BlendState.AlphaToCoverageEnable = blendState.AlphaToCoverageEnable;
			d3d12BlendState.IndependentBlendEnable = blendState.IndependentBlendEnable;

			// Number of Render Targets is hardcoded
			for (uint8_t rtDescIdx = 0; rtDescIdx < 8; ++rtDescIdx)
			{
				d3d12BlendState.RenderTarget[rtDescIdx].BlendEnable = blendState.RenderTarget[rtDescIdx].BlendEnable;
				d3d12BlendState.RenderTarget[rtDescIdx].LogicOpEnable = blendState.RenderTarget[rtDescIdx].LogicOpEnable;
				d3d12BlendState.RenderTarget[rtDescIdx].SrcBlend = ConvertToD3D12Blend(blendState.RenderTarget[rtDescIdx].SrcBlend);
				d3d12BlendState.RenderTarget[rtDescIdx].DestBlend = ConvertToD3D12Blend(blendState.RenderTarget[rtDescIdx].DestBlend);
				d3d12BlendState.RenderTarget[rtDescIdx].BlendOp = ConvertToD3D12BlendOp(blendState.RenderTarget[rtDescIdx].BlendOp);
				d3d12BlendState.RenderTarget[rtDescIdx].SrcBlendAlpha = ConvertToD3D12Blend(blendState.RenderTarget[rtDescIdx].SrcBlendAlpha);
				d3d12BlendState.RenderTarget[rtDescIdx].DestBlendAlpha = ConvertToD3D12Blend(blendState.RenderTarget[rtDescIdx].DestBlendAlpha);
				d3d12BlendState.RenderTarget[rtDescIdx].BlendOpAlpha = ConvertToD3D12BlendOp(blendState.RenderTarget[rtDescIdx].BlendOpAlpha);
				d3d12BlendState.RenderTarget[rtDescIdx].LogicOp = ConvertToD3D12LogicOp(blendState.RenderTarget[rtDescIdx].LogicOp);
			}

			return d3d12BlendState;
		}

		D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertToD3D12PrimitiveTopologyType(const PrimitiveTopologyType& primitiveTopologyType)
		{
			switch (primitiveTopologyType)
			{
			case PrimitiveTopologyType::Triangle:
				return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			}
		}

		D3D12_PRIMITIVE_TOPOLOGY ConvertToD3D12PrimitiveTopology(const PrimitiveTopology& primitiveTopology)
		{
			switch (primitiveTopology)
			{
			case PrimitiveTopology::TriangleList:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}

		D3D12_COMPARISON_FUNC ConvertToD3D12ComparisonFunc(const ComparisonFunc& compFunc)
		{
			switch (compFunc)
			{
			case ComparisonFunc::Less:
				return D3D12_COMPARISON_FUNC_LESS;
			case ComparisonFunc::Always:
				return D3D12_COMPARISON_FUNC_ALWAYS;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_COMPARISON_FUNC_LESS;
			}
		}

		D3D12_STENCIL_OP ConvertToD3D12StencilOp(const StencilOp& stencilOp)
		{
			switch (stencilOp)
			{
			case StencilOp::Keep:
				return D3D12_STENCIL_OP_KEEP;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return D3D12_STENCIL_OP_KEEP;
			}
		}

		D3D12_DEPTH_STENCIL_DESC ConvertToD3D12DepthStencilDesc(const DepthStencilDescription& dsDesc)
		{
			D3D12_DEPTH_STENCIL_DESC d3d12DsDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

			d3d12DsDesc.DepthEnable = dsDesc.DepthEnable;
			d3d12DsDesc.StencilEnable = dsDesc.StencilEnable;
			d3d12DsDesc.StencilReadMask = dsDesc.StencilReadMask;
			d3d12DsDesc.StencilWriteMask = dsDesc.StencilWriteMask;
			d3d12DsDesc.DepthFunc = ConvertToD3D12ComparisonFunc(dsDesc.DepthFunc);

			d3d12DsDesc.FrontFace.StencilFailOp = ConvertToD3D12StencilOp(dsDesc.FrontFace.StencilFailOp);
			d3d12DsDesc.FrontFace.StencilDepthFailOp = ConvertToD3D12StencilOp(dsDesc.FrontFace.StencilDepthFailOp);
			d3d12DsDesc.FrontFace.StencilPassOp = ConvertToD3D12StencilOp(dsDesc.FrontFace.StencilPassOp);
			d3d12DsDesc.FrontFace.StencilFunc = ConvertToD3D12ComparisonFunc(dsDesc.FrontFace.StencilFunc);

			d3d12DsDesc.BackFace.StencilFailOp = ConvertToD3D12StencilOp(dsDesc.BackFace.StencilFailOp);
			d3d12DsDesc.BackFace.StencilDepthFailOp = ConvertToD3D12StencilOp(dsDesc.BackFace.StencilDepthFailOp);
			d3d12DsDesc.BackFace.StencilPassOp = ConvertToD3D12StencilOp(dsDesc.BackFace.StencilPassOp);
			d3d12DsDesc.BackFace.StencilFunc = ConvertToD3D12ComparisonFunc(dsDesc.BackFace.StencilFunc);

			return d3d12DsDesc;
		}

		D3D12_CLEAR_FLAGS ConvertToD3D12ClearFlag(ClearFlags::Flag clearFlags)
		{
			D3D12_CLEAR_FLAGS d3d12ClearFlags = D3D12_CLEAR_FLAG_DEPTH;

			d3d12ClearFlags = clearFlags & ClearFlags::Depth ? (d3d12ClearFlags | D3D12_CLEAR_FLAG_DEPTH) : (d3d12ClearFlags & ~D3D12_CLEAR_FLAG_DEPTH);
			d3d12ClearFlags = clearFlags & ClearFlags::Stencil ? (d3d12ClearFlags | D3D12_CLEAR_FLAG_STENCIL) : d3d12ClearFlags;

			return d3d12ClearFlags;
		}
	}
}