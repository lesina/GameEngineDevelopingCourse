#include <D3D12RHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHIDevice::D3D12RHIDevice(D3D12RHIFactory::Ptr factory)
		{
			HRESULT hr = D3D12CreateDevice(
				nullptr,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_NativeDevice));

			if (FAILED(hr))
			{
				RefCountPtr<IDXGIAdapter> warpAdapter;
				hr = factory->GetHandle()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
				assert(SUCCEEDED(hr));

				hr = D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_NativeDevice));

				assert(SUCCEEDED(hr));
			}
		}

		RenderNativeObject D3D12RHIDevice::GetNativeObject()
		{
			return RenderNativeObject(m_NativeDevice.Get());
		}

		RefCountPtr<ID3D12Device> D3D12RHIDevice::GetHandle() const
		{
			return m_NativeDevice;
		}
	}
}