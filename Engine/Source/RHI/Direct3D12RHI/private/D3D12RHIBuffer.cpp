#include <D3D12RHIBuffer.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHIBuffer::D3D12RHIBuffer(
			const Description& desc,
			RefCountPtr<ID3D12Resource> resourceDefault,
			RefCountPtr<ID3D12Resource> resourceUpload
		)
			: RHIBuffer(desc)
		{
			m_NativeResourceDefault = resourceDefault;
			m_NativeResourceUpload = resourceUpload;
		}

		void D3D12RHIBuffer::CopyData(int elementIndex, void* data, uint32_t DataSize)
		{
			assert(m_NativeResourceUpload != nullptr);
			assert(m_Description.UsageFlag != RHIBuffer::UsageFlag::GpuReadOnly);

			uint8_t* mappedData;
			HRESULT hr = m_NativeResourceUpload->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));
			assert(SUCCEEDED(hr));

			memcpy(&mappedData[elementIndex * m_Description.ElementSize], data, DataSize);

			m_NativeResourceUpload->Unmap(0, nullptr);

			mappedData = nullptr;
		}

		RenderNativeObject D3D12RHIBuffer::GetNativeObject()
		{
			if (m_NativeResourceDefault != nullptr)
			{
				return RenderNativeObject(m_NativeResourceDefault.Get());
			}

			return RenderNativeObject(m_NativeResourceUpload.Get());
		}

		RefCountPtr<ID3D12Resource> D3D12RHIBuffer::GetHandle() const
		{
			if (m_NativeResourceDefault != nullptr)
			{
				return m_NativeResourceDefault;
			}

			return m_NativeResourceUpload;
		}
	}
}