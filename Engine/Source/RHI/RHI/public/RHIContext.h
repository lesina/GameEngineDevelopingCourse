#pragma once

#include <Geometry.h>
#include <RHI/RHI/export.h>
#include <Buffer.h>
#include <RHICore.h>
#include <Mesh.h>
#include <Device.h>
#include <Factory.h>
#include <SwapChain.h>
#include <Fence.h>
#include <CommandQueue.h>
#include <CommandAllocator.h>
#include <CommandList.h>
#include <Texture.h>
#include <Technique.h>
#include <PipelineStateObject.h>

namespace cqe
{
	namespace Render::RHI
	{
		enum class Type : uint8_t
		{
			D3D12 = 0,
		};

		const std::unordered_map<std::string, Type> k_RHITypeMap =
		{
			{"D3D12", Type::D3D12},
		};

		class Context
		{
		public:
			using Ptr = std::shared_ptr<Context>;

		public:
			virtual Texture* CreateTexture(const Texture::Description& description) = 0;
			virtual void FreeTexture(const Texture* texture, Texture::UsageFlags::Flag usageFlag) = 0;
			virtual void LoadTextureFromFile(Texture* texture, const std::wstring&& filePath) = 0;
			virtual Buffer::Ptr CreateBuffer(Buffer::Description&& description) = 0;
			virtual Technique::Ptr CreateTechnique(
				const Technique::ShaderInfo& shaderInfo,
				const Technique::InputLayout& inputLayout,
				const Technique::RootSignatureDescription& rootSignatureDescription
			) = 0;
			virtual PipelineStateObject::Ptr CreatePSO(const PipelineStateObject::Description& description) = 0;
			virtual Mesh* CreateMesh(
				const Mesh::VertexBufferDescription& vertexDesc,
				const Mesh::IndexBufferDescription& indexDesc
			) = 0;
			virtual void SetDescriptorHeaps() = 0;
			[[nodiscard]] virtual bool CheckMinimalRequirements() const = 0;

		public:
			virtual Device::Ptr GetDevice() const = 0;
			virtual Factory::Ptr GetFactory() const = 0;
			virtual SwapChain::Ptr GetSwapChain() const = 0;
			virtual Fence::Ptr GetFence() const = 0;
			virtual CommandQueue::Ptr GetCommandQueue() const = 0;
			virtual CommandList::Ptr GetCommandList() const = 0;

		protected:
			Context() = default;
		};
	}
}