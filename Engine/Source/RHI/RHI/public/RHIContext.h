#pragma once

#include <Geometry.h>
#include <RHI/RHI/export.h>
#include <RHIBuffer.h>
#include <RHICore.h>
#include <RHIMesh.h>
#include <RHIDevice.h>
#include <RHIFactory.h>
#include <RHISwapChain.h>
#include <RHIFence.h>
#include <RHICommandQueue.h>
#include <RHICommandAllocator.h>
#include <RHICommandList.h>
#include <RHITexture.h>
#include <RHITechnique.h>
#include <RHIPipelineStateObject.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		enum class RHIType : uint8_t
		{
			D3D12 = 0,
			Vulkan,
		};

		const std::unordered_map<std::string, RHIType> k_RHITypeMap =
		{
			{"D3D12", RHIType::D3D12},
			{"Vulkan", RHIType::Vulkan},
		};

		class RHIContext
		{
		public:
			using Ptr = std::shared_ptr<RHIContext>;

		public:
			virtual RHITexture::Ptr CreateTexture(const RHITexture::Description& description) = 0;
			virtual RHIBuffer::Ptr CreateBuffer(RHIBuffer::Description&& description) = 0;
			virtual RHITechnique::Ptr CreateTechnique(
				const RHITechnique::ShaderInfo& shaderInfo,
				const RHITechnique::InputLayout& inputLayout,
				const RHITechnique::RootSignature& rootSignature
			) = 0;
			virtual RHIPipelineStateObject::Ptr CreatePSO(const RHIPipelineStateObject::Description& description) = 0;
			virtual RHIMesh::Ptr CreateMesh(
				const RHIMesh::VertexBufferDescription& vertexDesc,
				const RHIMesh::IndexBufferDescription& indexDesc
			) = 0;

		public:
			virtual RHIDevice::Ptr GetDevice() const = 0;
			virtual RHIFactory::Ptr GetFactory() const = 0;
			virtual RHISwapChain::Ptr GetSwapChain() const = 0;
			virtual RHIFence::Ptr GetFence() const = 0;
			virtual RHICommandQueue::Ptr GetCommandQueue() const = 0;
			virtual RHICommandList::Ptr GetCommandList() const = 0;

		protected:
			RHIContext() = default;
		};
	}
}