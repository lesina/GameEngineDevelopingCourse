#pragma once

#include <RHICommon.h>
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
            static RefCountPtr<ID3DBlob> CompileShader(
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

                RefCountPtr<ID3DBlob> byteCode = nullptr;
                RefCountPtr<ID3DBlob> errors;
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