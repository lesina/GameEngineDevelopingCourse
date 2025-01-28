#pragma once

#include <Windows.h>
#include <dxcapi.h>

#include <RHITechnique.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class VulkanShaderCompiler final
		{
		public:
			using BlobSPIRV = std::vector<uint32_t>;

			VulkanShaderCompiler();

		public:
			[[nodiscard]] BlobSPIRV CompileFromFile(const RHITechnique::ShaderInfoDescription& shaderDesc);

		private:
			RefCountPtr<IDxcLibrary> m_DxcLibrary = nullptr;
			RefCountPtr<IDxcIncludeHandler> m_DxcIncludeHandler = nullptr;
			RefCountPtr<IDxcCompiler> m_DxcCompiler = nullptr;
		};
	}
}