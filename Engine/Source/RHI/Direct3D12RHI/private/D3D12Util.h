#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <d3dx12.h>
#include <DDSTextureLoader.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		struct D3D12Util
		{
			static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device,
				ID3D12GraphicsCommandList* cmdList,
				const void* initData,
				UINT64 byteSize,
				Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
			{
                Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

                D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
                D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
                // Create the actual default buffer resource.
                HRESULT hr = device->CreateCommittedResource(
                    &HeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_COMMON,
                    nullptr,
                    IID_PPV_ARGS(defaultBuffer.GetAddressOf()));
                assert(SUCCEEDED(hr));

                // In order to copy CPU memory data into our default buffer, we need to create
                // an intermediate upload heap.
                D3D12_HEAP_PROPERTIES UploadHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
                hr = device->CreateCommittedResource(
                    &UploadHeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
                assert(SUCCEEDED(hr));

                // Describe the data we want to copy into the default buffer.
                D3D12_SUBRESOURCE_DATA subResourceData = {};
                subResourceData.pData = initData;
                subResourceData.RowPitch = byteSize;
                subResourceData.SlicePitch = subResourceData.RowPitch;

                // Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
                // will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
                // the intermediate upload heap data will be copied to mBuffer.
                D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
                    D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
                cmdList->ResourceBarrier(1, &ResBarrierTransition);
                UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

                ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
                    D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
                cmdList->ResourceBarrier(1, &ResBarrierTransition);

                // Note: uploadBuffer has to be kept alive after the above function calls because
                // the command list has not been executed yet that performs the actual copy.
                // The caller can Release the uploadBuffer after it knows the copy has been executed.

                return defaultBuffer;
			}

            static uint32_t CalcConstantBufferByteSize(uint32_t byteSize)
            {
                // Constant buffers must be a multiple of the minimum hardware
                // allocation size (usually 256 bytes).  So round up to nearest
                // multiple of 256.  We do this by adding 255 and then masking off
                // the lower 2 bytes which store all bits < 256.
                // Example: Suppose byteSize = 300.
                // (300 + 255) & ~255
                // 555 & ~255
                // 0x022B & ~0x00ff
                // 0x022B & 0xff00
                // 0x0200
                // 512
                return (byteSize + 255) & ~255;
            }

            static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
                const std::wstring& filename,
                const D3D_SHADER_MACRO* defines,
                const std::string& entrypoint,
                const std::string& target)
            {
                UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
                compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

                HRESULT hr = S_OK;

                Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
                Microsoft::WRL::ComPtr<ID3DBlob> errors;
                hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

                if (errors != nullptr)
                    OutputDebugStringA((char*)errors->GetBufferPointer());

                assert(SUCCEEDED(hr));

                return byteCode;
            }
		};
	}
}