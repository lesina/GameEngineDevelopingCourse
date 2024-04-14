#pragma once

#include <RHIAdapter.h>
#include <RHI/Direct3D12RHI/export.h>
#include <Mesh.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIPrivate;

		class DIRECT3D_API D3D12RHI final : public RHIAdapter
		{
		public:
			using Ptr = std::shared_ptr<RHIAdapter>;

		public:
			D3D12RHI();

		public:
			virtual void Init() override;
			virtual void Update(Mesh::Ptr mesh, Material::Ptr material) override;
			virtual Mesh::Ptr CreateBoxMesh() override;
			virtual Material::Ptr GetMaterial(const std::string& name) override;
			virtual void ExecuteCommandLists() override;
			virtual void Flush() override;
			virtual void OnResize() override;

		private:
			std::shared_ptr<D3D12RHIPrivate> m_d3d12Private = nullptr;
		};
	}
}