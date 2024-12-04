#pragma once

#include <Debug/Assertions.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <RHICore.h>
#include <RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		DXGI_FORMAT ConvertToDXGIFormat(const ResourceFormat& resourceFormat);
		ResourceFormat ConvertToResourceFormat(DXGI_FORMAT resourceFormat);
		std::string GetShaderTarget(const RHITechnique::ShaderInfoDescription::ShaderType& shaderType);
		D3D12_INPUT_CLASSIFICATION ConvertToD3D12InputClassification(const RHITechnique::InputLayoutDescription::Classification& classification);
		D3D12_RASTERIZER_DESC ConvertToD3D12RasterState(const RasterizerDescription& rasterState);
		D3D12_BLEND ConvertToD3D12Blend(const Blend& blend);
		D3D12_BLEND_OP ConvertToD3D12BlendOp(const BlendOperation& blendOp);
		D3D12_LOGIC_OP ConvertToD3D12LogicOp(const LogicOperation& logicOp);
		D3D12_BLEND_DESC ConvertToD3D12BlendState(const BlendDescription& blendState);
		D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertToD3D12PrimitiveTopologyType(const PrimitiveTopologyType& primitiveTopologyType);
		D3D12_PRIMITIVE_TOPOLOGY ConvertToD3D12PrimitiveTopology(const PrimitiveTopology& primitiveTopology);
		D3D12_COMPARISON_FUNC ConvertToD3D12ComparisonFunc(const ComparisonFunc& compFunc);
		D3D12_STENCIL_OP ConvertToD3D12StencilOp(const StencilOp& stencilOp);
		D3D12_DEPTH_STENCIL_DESC ConvertToD3D12DepthStencilDesc(const DepthStencilDescription& dsDesc);
		D3D12_CLEAR_FLAGS ConvertToD3D12ClearFlag(ClearFlags::Flag clearFlags);
	}
}