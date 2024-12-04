#include <D3D12RHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHIFactory::D3D12RHIFactory()
		{
			HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_NativeFactory));
			assert(SUCCEEDED(hr));
		}

		RenderNativeObject D3D12RHIFactory::GetNativeObject()
		{
			return RenderNativeObject(m_NativeFactory.Get());
		}

		RefCountPtr<IDXGIFactory4> D3D12RHIFactory::GetHandle() const
		{
			return m_NativeFactory;
		}
	}
}