#include <d3dx12.h>

#include <D3D12DescriptorHeap.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12DescriptorHeap::D3D12DescriptorHeap(
			D3D12RHIDevice::Ptr device,
			uint32_t NumDescriptors,
			D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType
		) 
			: m_DescriptorHeapType(descriptorHeapType)
			, m_MaximumDescriptors(NumDescriptors)
		{
			m_DescriptorSize = device->GetHandle()->GetDescriptorHandleIncrementSize(descriptorHeapType);

			D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
			HeapDesc.NumDescriptors = NumDescriptors;
			HeapDesc.Type = descriptorHeapType;
			HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HeapDesc.NodeMask = 0;
			HRESULT hr = device->GetHandle()->CreateDescriptorHeap(
				&HeapDesc, IID_PPV_ARGS(m_NativeHeap.GetAddressOf()));
			assert(SUCCEEDED(hr));
		}

		[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::AllocateNewDescriptor()
		{
			assert(m_HeapIdx < m_MaximumDescriptors);

			CD3DX12_CPU_DESCRIPTOR_HANDLE CpuDescriptorHandle(m_NativeHeap->GetCPUDescriptorHandleForHeapStart());
			CpuDescriptorHandle.Offset(m_HeapIdx++, m_DescriptorSize);

			return CpuDescriptorHandle;
		}

		RenderNativeObject D3D12DescriptorHeap::GetNativeObject()
		{
			return RenderNativeObject(m_NativeHeap.Get());
		}

		RefCountPtr<ID3D12DescriptorHeap> D3D12DescriptorHeap::GetHandle() const
		{
			return m_NativeHeap;
		}
	}
}