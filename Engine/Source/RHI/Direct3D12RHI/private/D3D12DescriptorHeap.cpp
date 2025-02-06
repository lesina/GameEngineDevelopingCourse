#include <d3dx12.h>

#include <D3D12RHIDevice.h>
#include <D3D12DescriptorHeap.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12DescriptorHeap::D3D12DescriptorHeap(
			RefCountPtr<D3D12RHIDevice> device,
			uint32_t NumDescriptors,
			D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType
		) 
			: m_DescriptorHeapType(descriptorHeapType)
			, m_MaximumDescriptors(NumDescriptors)
		{
			m_DescriptorSize = device->GetHandle()->GetDescriptorHandleIncrementSize(descriptorHeapType);

			m_ShaderVisible = descriptorHeapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

			D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
			HeapDesc.NumDescriptors = NumDescriptors;
			HeapDesc.Type = descriptorHeapType;
			HeapDesc.Flags = m_ShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HeapDesc.NodeMask = 0;
			HRESULT hr = device->GetHandle()->CreateDescriptorHeap(
				&HeapDesc, IID_PPV_ARGS(m_NativeHeap.GetAddressOf()));
			assert(SUCCEEDED(hr));

			CD3DX12_CPU_DESCRIPTOR_HANDLE CpuDescriptorHandle(m_NativeHeap->GetCPUDescriptorHandleForHeapStart());
			for (uint32_t DescriptorIdx = 0; DescriptorIdx < m_MaximumDescriptors; ++DescriptorIdx)
			{
				m_AvailableCPUDescriptors.push(CpuDescriptorHandle);

				CpuDescriptorHandle.Offset(1, m_DescriptorSize);
			}

			if (m_ShaderVisible)
			{
				CD3DX12_GPU_DESCRIPTOR_HANDLE GpuDescriptorHandle(m_NativeHeap->GetGPUDescriptorHandleForHeapStart());
				for (uint32_t DescriptorIdx = 0; DescriptorIdx < m_MaximumDescriptors; ++DescriptorIdx)
				{
					m_AvailableGPUDescriptors.push(GpuDescriptorHandle);

					GpuDescriptorHandle.Offset(1, m_DescriptorSize);
				}
			}

			assert(m_AvailableCPUDescriptors.size() <= m_MaximumDescriptors);
			assert(m_AvailableGPUDescriptors.size() <= m_MaximumDescriptors);
		}

		void D3D12DescriptorHeap::Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle)
		{
			assert(m_AvailableCPUDescriptors.size() > 0);
			assert(cpuHandle);

			*cpuHandle = m_AvailableCPUDescriptors.top();
			m_AvailableCPUDescriptors.pop();

			if (m_ShaderVisible)
			{
				assert(m_AvailableGPUDescriptors.size() > 0);
				assert(gpuHandle);

				*gpuHandle = m_AvailableGPUDescriptors.top();
				m_AvailableGPUDescriptors.pop();
			}
		}

		void D3D12DescriptorHeap::Free(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
		{
			m_AvailableCPUDescriptors.push(cpuHandle);
			
			if (m_ShaderVisible)
			{
				m_AvailableGPUDescriptors.push(gpuHandle);
			}
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