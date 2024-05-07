#pragma once

#include <Geometry.h>
#include <RenderData.h>
#include <RHI/RHI/export.h>
#include <RHIMesh.h>
#include <RHIMaterial.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		enum class RHIType : uint8_t
		{
			D3D12 = 0,
		};

		const std::unordered_map<std::string, RHIType> k_RHITypeMap =
		{
			{"D3D12", RHIType::D3D12},
		};

		class RHIAdapter
		{
		public:
			using Ptr = std::shared_ptr<RHIAdapter>;

		public:
			virtual void Init() = 0;
			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;
			virtual void Draw(RenderData* renderData, size_t frame) = 0;
			virtual void CreateMesh(RenderCore::Geometry::Ptr geometry, RHIMesh::ID& meshID, RHIMaterial::ID& materialID) = 0;
			virtual void ExecuteCommandLists() = 0;
			virtual void Flush() = 0;
			virtual void OnResize() = 0;

		protected:
			RHIAdapter() = default;

		protected:
			std::vector<RHIMesh*> m_Meshes;
			std::vector<RHIMaterial*> m_Materials;
		};
	}
}