#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <RHIPipelineStateObject.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIPipelineStateObject final : public RHIPipelineStateObject
		{
		public:
			using Ptr = RefCountPtr<D3D12RHIPipelineStateObject>;

		public:
			D3D12RHIPipelineStateObject() = delete;
			D3D12RHIPipelineStateObject(const Description& desc, RefCountPtr<ID3D12PipelineState> pso);

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12PipelineState> GetHandle() const;

		private:
			RefCountPtr<ID3D12PipelineState> m_NativePSO = nullptr;
		};
	}
}