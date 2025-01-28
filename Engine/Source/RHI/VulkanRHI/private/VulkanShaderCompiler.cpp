#include <FileSystem.h>
#include <VulkanShaderCompiler.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		static LPCWSTR ConvertToDxcTargetProfile(RHITechnique::ShaderInfoDescription::ShaderType shaderType)
		{
			switch (shaderType)
			{
			case RHITechnique::ShaderInfoDescription::ShaderType::VertexShader:
				return L"vs_6_8";
			case RHITechnique::ShaderInfoDescription::ShaderType::PixelShader:
				return L"ps_6_8";
			default:
				ASSERT_NOT_IMPLEMENTED;
				return L"vs_6_8";
			}
		}

		VulkanShaderCompiler::VulkanShaderCompiler()
		{
			HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(m_DxcLibrary.GetAddressOf()));
			assert(SUCCEEDED(hr));

			hr = m_DxcLibrary->CreateIncludeHandler(m_DxcIncludeHandler.GetAddressOf());
			assert(SUCCEEDED(hr));

			hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(m_DxcCompiler.GetAddressOf()));
			assert(SUCCEEDED(hr));
		}

		VulkanShaderCompiler::BlobSPIRV VulkanShaderCompiler::CompileFromFile(const RHITechnique::ShaderInfoDescription& shaderDesc)
		{
			RefCountPtr<IDxcBlobEncoding> blobEncoding = nullptr;
			RefCountPtr<IDxcOperationResult> dxcResult = nullptr;
			RefCountPtr<IDxcBlob> dxcBlob = nullptr;

			std::wstring filename = Core::g_FileSystem->GetShaderPath(shaderDesc.ShaderFile);
			std::wstring entryPoint(shaderDesc.EntryPoint.begin(), shaderDesc.EntryPoint.end());

			UINT32 codePage = CP_UTF8;
			HRESULT hr = m_DxcLibrary->CreateBlobFromFile(filename.data(), &codePage, blobEncoding.ReleaseAndGetAddressOf());
			assert(SUCCEEDED(hr));

			LPCWSTR targetProfile = ConvertToDxcTargetProfile(shaderDesc.Type);

			// NOTE: DXC requires const wchar**, and not const wchar* const *
			static LPCWSTR args[] =
			{
				L"-spirv",
				L"-T",
				targetProfile,
				L"-Zi"
			};

			hr = m_DxcCompiler->Compile(blobEncoding, filename.data(), entryPoint.data(), targetProfile, args,
				static_cast<uint32_t>(std::size(args)), nullptr, 0, m_DxcIncludeHandler, dxcResult.ReleaseAndGetAddressOf());
			assert(SUCCEEDED(hr));

			dxcResult->GetResult(dxcBlob.ReleaseAndGetAddressOf());

			const size_t spirvSizeBytes = dxcBlob->GetBufferSize();
			assert(spirvSizeBytes > 0 && (spirvSizeBytes % 4) == 0);

			BlobSPIRV spirv(spirvSizeBytes / 4);
			memcpy(spirv.data(), dxcBlob->GetBufferPointer(), spirvSizeBytes);

			return spirv;
		}
	}
}