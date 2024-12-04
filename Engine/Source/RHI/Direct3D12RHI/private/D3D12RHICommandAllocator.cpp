#include <D3D12RHICommandAllocator.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHICommandAllocator::D3D12RHICommandAllocator(D3D12RHIDevice::Ptr device)
		{
			HRESULT hr = device->GetHandle()->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_NativeCommandAllocator.GetAddressOf()));
			
			assert(SUCCEEDED(hr));
		}

		RenderNativeObject D3D12RHICommandAllocator::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandAllocator.Get());
		}

		RefCountPtr<ID3D12CommandAllocator> D3D12RHICommandAllocator::GetHandle() const
		{
			return m_NativeCommandAllocator;
		}

		void D3D12RHICommandAllocator::Reset()
		{
			HRESULT hr = m_NativeCommandAllocator->Reset();
			assert(SUCCEEDED(hr));
		}
	}
}