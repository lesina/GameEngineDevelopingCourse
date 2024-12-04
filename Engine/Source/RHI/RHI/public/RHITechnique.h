#pragma once

#include <RHICommon.h>
#include <RHICore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		// It represents render backend resource so we still inherit it from RenderBackendResource
		class RHITechnique : public RefCounter<RenderBackendResource>
		{
		public:
			struct ShaderInfoDescription
			{
				enum class ShaderType
				{
					VertexShader = 0,
					PixelShader,

					Undefined
				};

				ShaderType Type;
				std::string ShaderFile;
				std::string EntryPoint;
			};
			using ShaderInfo = std::vector<ShaderInfoDescription>;

			struct InputLayoutDescription
			{
				enum class Classification
				{
					PerVertex = 0,
					PerInstance
				};

				std::string SemanticName;
				uint32_t Index;
				ResourceFormat Format;
				uint32_t InputSlot;
				Classification InputSlotClass;
				uint32_t InstanceDataStepRate;
			};
			using InputLayout = std::vector<InputLayoutDescription>;

			struct RootSignatureDescription
			{
				uint32_t SlotIndex;
				uint32_t SpaceIndex;
				bool IsConstantBuffer;
			};
			using RootSignature = std::vector<RootSignatureDescription>;

		public:
			using Ptr = RefCountPtr<RHITechnique>;

		public:
			RHITechnique() = delete;
			RHITechnique(
				const ShaderInfo& shaderInfo,
				const InputLayout& inputLayout,
				const RootSignature& rootSignature
			)
				: m_ShaderInfo(shaderInfo)
				, m_InputLayout(inputLayout)
				, m_RootSignature(rootSignature)
			{
			}

		protected:
			ShaderInfo m_ShaderInfo;
			InputLayout m_InputLayout;
			RootSignature m_RootSignature;
		};
	}
}