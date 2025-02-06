#pragma once

#include <RHI/Direct3D12RHI/export.h>

#include <Debug/Assertions.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <RHICore.h>
#include <RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		DIRECT3D_API DXGI_FORMAT ConvertToDXGIFormat(const ResourceFormat& resourceFormat);
		DIRECT3D_API ResourceFormat ConvertToResourceFormat(DXGI_FORMAT resourceFormat);
		DIRECT3D_API std::string GetShaderTarget(const RHITechnique::ShaderInfoDescription::ShaderType& shaderType);
		DIRECT3D_API D3D12_INPUT_CLASSIFICATION ConvertToD3D12InputClassification(const RHITechnique::InputLayoutDescription::Classification& classification);
		DIRECT3D_API D3D12_RASTERIZER_DESC ConvertToD3D12RasterState(const RasterizerDescription& rasterState);
		DIRECT3D_API D3D12_BLEND ConvertToD3D12Blend(const Blend& blend);
		DIRECT3D_API D3D12_BLEND_OP ConvertToD3D12BlendOp(const BlendOperation& blendOp);
		DIRECT3D_API D3D12_LOGIC_OP ConvertToD3D12LogicOp(const LogicOperation& logicOp);
		DIRECT3D_API D3D12_BLEND_DESC ConvertToD3D12BlendState(const BlendDescription& blendState);
		DIRECT3D_API D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertToD3D12PrimitiveTopologyType(const PrimitiveTopologyType& primitiveTopologyType);
		DIRECT3D_API D3D12_PRIMITIVE_TOPOLOGY ConvertToD3D12PrimitiveTopology(const PrimitiveTopology& primitiveTopology);
		DIRECT3D_API D3D12_COMPARISON_FUNC ConvertToD3D12ComparisonFunc(const ComparisonFunc& compFunc);
		DIRECT3D_API D3D12_STENCIL_OP ConvertToD3D12StencilOp(const StencilOp& stencilOp);
		DIRECT3D_API D3D12_DEPTH_STENCIL_DESC ConvertToD3D12DepthStencilDesc(const DepthStencilDescription& dsDesc);
		DIRECT3D_API D3D12_CLEAR_FLAGS ConvertToD3D12ClearFlag(ClearFlags::Flag clearFlags);
	}
}