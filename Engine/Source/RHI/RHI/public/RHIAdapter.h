#pragma once

#include <RHI/RHI/export.h>
#include <Window/IWindow.h>
#include <Mesh.h>
#include <Material.h>

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
			virtual void Update(Mesh::Ptr mesh, Material::Ptr material) = 0;
			virtual Mesh::Ptr CreateBoxMesh() = 0;
			virtual Material::Ptr GetMaterial(const std::string& name) = 0;
			virtual void ExecuteCommandLists() = 0;
			virtual void Flush() = 0;
			virtual void OnResize() = 0;

		protected:
			RHIAdapter() = default;
		};
	}
}