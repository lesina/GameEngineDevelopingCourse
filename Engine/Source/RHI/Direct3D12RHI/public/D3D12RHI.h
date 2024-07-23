#pragma once

#include <Geometry.h>
#include <RHIAdapter.h>
#include <RHI/Direct3D12RHI/export.h>
#include <RHIMesh.h>
#include <RHIMaterial.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIPrivate;

		class DIRECT3D_API D3D12RHI final : public RHIAdapter
		{
		public:
			D3D12RHI();
			~D3D12RHI();

		public:
			virtual void Init() override;
			virtual void BeginFrame() override;
			virtual void EndFrame() override;
			virtual void Draw(RenderData* renderData, size_t frame) override;
			virtual void CreateMesh(RenderCore::Geometry* geometry, RHIMesh::ID& meshID, RHIMaterial::ID& materialID) override;
			virtual void ExecuteCommandLists() override;
			virtual void Flush() override;
			virtual void OnResize() override;

		private:
			D3D12RHIPrivate* m_d3d12Private = nullptr;
		};
	}
}