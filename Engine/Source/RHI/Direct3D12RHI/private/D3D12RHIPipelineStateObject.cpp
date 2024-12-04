#include <D3D12RHIPipelineStateObject.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHIPipelineStateObject::D3D12RHIPipelineStateObject(
			const Description& desc,
			RefCountPtr<ID3D12PipelineState> pso
		)
			: RHIPipelineStateObject(desc)
			, m_NativePSO(pso)
		{

		}

		RenderNativeObject D3D12RHIPipelineStateObject::GetNativeObject()
		{
			return RenderNativeObject(m_NativePSO.Get());
		}

		RefCountPtr<ID3D12PipelineState> D3D12RHIPipelineStateObject::GetHandle() const
		{
			return m_NativePSO;
		}
	}
}