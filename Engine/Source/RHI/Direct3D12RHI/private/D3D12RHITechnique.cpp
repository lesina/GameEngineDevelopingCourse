#include <D3D12RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHITechnique::D3D12RHITechnique(
			const ShaderInfo& shaderInfo,
			const InputLayout& inputLayout,
			const RootSignature& rootSignature,
			RefCountPtr<ID3D12RootSignature> d3d12RootSiungature,
			ShaderBlobList&& shaderBlobList,
			InputLayoutList&& inputLayoutList
		) 
			: RHITechnique(shaderInfo, inputLayout, rootSignature)
			, m_d3d12RootSignature(d3d12RootSiungature)
			, m_ShaderBlobList(std::move(shaderBlobList))
			, m_InputLayoutList(std::move(inputLayoutList))
		{
		}


		RenderNativeObject D3D12RHITechnique::GetNativeObject()
		{
			assert(0 && "D3D12RHITechnique can't have the native object");
			return nullptr;
		}
	};
}