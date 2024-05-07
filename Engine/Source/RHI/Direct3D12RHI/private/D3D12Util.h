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
                uint32_t byteSize,
				Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
			{
                Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

                D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
                D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
                HRESULT hr = device->CreateCommittedResource(
                    &HeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_COMMON,
                    nullptr,
                    IID_PPV_ARGS(defaultBuffer.GetAddressOf()));
                assert(SUCCEEDED(hr));

                D3D12_HEAP_PROPERTIES UploadHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
                hr = device->CreateCommittedResource(
                    &UploadHeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &ResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
                assert(SUCCEEDED(hr));

                D3D12_SUBRESOURCE_DATA subResourceData = {};
                subResourceData.pData = initData;
                subResourceData.RowPitch = byteSize;
                subResourceData.SlicePitch = subResourceData.RowPitch;

                D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
                    D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
                cmdList->ResourceBarrier(1, &ResBarrierTransition);
                UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

                ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
                    D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
                cmdList->ResourceBarrier(1, &ResBarrierTransition);

                return defaultBuffer;
			}

            static uint32_t CalcConstantBufferByteSize(uint32_t byteSize)
            {
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