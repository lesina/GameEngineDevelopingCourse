#pragma once

#include <D3D12Util.h>

namespace GameEngine
{
    namespace Render::HAL
    {
        template<typename T>
        class UploadBuffer
        {
        public:
            UploadBuffer(ID3D12Device* device, size_t elementCount, bool isConstantBuffer) :
                m_IsConstantBuffer(isConstantBuffer)
            {
                m_ElementByteSize = sizeof(T);

                if (isConstantBuffer)
                {
                    m_ElementByteSize = D3D12Util::CalcConstantBufferByteSize(sizeof(T));
                }

                D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
                D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * elementCount);
                HRESULT hr = device->CreateCommittedResource(
                    &HeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&m_UploadBuffer));
                assert(SUCCEEDED(hr));

                hr = m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData));
                assert(SUCCEEDED(hr));
            }

            UploadBuffer(const UploadBuffer& rhs) = delete;
            UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
            ~UploadBuffer()
            {
                if (m_UploadBuffer != nullptr)
                    m_UploadBuffer->Unmap(0, nullptr);

                m_MappedData = nullptr;
            }

            ID3D12Resource* Resource()const
            {
                return m_UploadBuffer.Get();
            }

            void CopyData(int elementIndex, const T& data)
            {
                memcpy(&m_MappedData[elementIndex * m_ElementByteSize], &data, sizeof(T));
            }

            size_t GetElementByteSize() const
            {
                return m_ElementByteSize;
            }

        private:
            Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;
            BYTE* m_MappedData = nullptr;

            size_t m_ElementByteSize = 0;
            bool m_IsConstantBuffer = false;
        };
    }
}