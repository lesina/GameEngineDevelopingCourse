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
            UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
                mIsConstantBuffer(isConstantBuffer)
            {
                mElementByteSize = sizeof(T);

                if (isConstantBuffer)
                {
                    mElementByteSize = D3D12Util::CalcConstantBufferByteSize(sizeof(T));
                }

                D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
                D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
                HRESULT hr = device->CreateCommittedResource(
                    &HeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&mUploadBuffer));
                assert(SUCCEEDED(hr));

                hr = mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
                assert(SUCCEEDED(hr));
            }

            UploadBuffer(const UploadBuffer& rhs) = delete;
            UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
            ~UploadBuffer()
            {
                if (mUploadBuffer != nullptr)
                    mUploadBuffer->Unmap(0, nullptr);

                mMappedData = nullptr;
            }

            ID3D12Resource* Resource()const
            {
                return mUploadBuffer.Get();
            }

            void CopyData(int elementIndex, const T& data)
            {
                memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
            }

        private:
            Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
            BYTE* mMappedData = nullptr;

            UINT mElementByteSize = 0;
            bool mIsConstantBuffer = false;
        };
    }
}