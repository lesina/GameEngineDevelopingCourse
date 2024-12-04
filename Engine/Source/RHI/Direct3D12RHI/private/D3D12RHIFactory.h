#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include <RHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIFactory final : public RHIFactory
		{
		public:
			using Ptr = RefCountPtr<D3D12RHIFactory>;

		public:
			D3D12RHIFactory();
			~D3D12RHIFactory() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<IDXGIFactory4> GetHandle() const;

		private:
			RefCountPtr<IDXGIFactory4> m_NativeFactory = nullptr;
		};
	}
}