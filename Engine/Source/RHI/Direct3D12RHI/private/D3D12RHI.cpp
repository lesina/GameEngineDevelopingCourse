#include <array.h>
#include <Geometry.h>
#include <D3D12Mesh.h>
#include <D3D12RHI.h>
#include <D3D12RHIPrivate.h>
#include <D3D12Material.h>
#include <Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHI::D3D12RHI() :
			m_d3d12Private(new D3D12RHIPrivate())
		{
			m_d3d12Private->EnableDebugLayer();
		}

		D3D12RHI::~D3D12RHI()
		{
			delete m_d3d12Private;
		}

		void D3D12RHI::Init()
		{
			m_d3d12Private->Init();
		}

		void D3D12RHI::BeginFrame()
		{
			m_d3d12Private->BeginFrame();
		}

		void D3D12RHI::EndFrame()
		{
			m_d3d12Private->EndFrame();
		}

		void D3D12RHI::Draw(RenderData* renderData, size_t frame)
		{
			RHIMesh::ID meshID = renderData->GetMesh()->GetID();
			RHIMaterial::ID materialID = renderData->GetMaterial()->GetID();

			assert(meshID != RHIMesh::k_invalidMeshID);
			assert(meshID < m_Meshes.size());
			assert(materialID != RHIMaterial::k_invalidMaterialID);
			assert(materialID < m_Materials.size());

			m_d3d12Private->Draw(
				meshID,
				reinterpret_cast<D3D12Mesh*>(m_Meshes[meshID]),
				materialID,
				reinterpret_cast<D3D12Material*>(m_Materials[materialID]),
				renderData->GetPosition(frame)
			);
		}

		void D3D12RHI::CreateMesh(RenderCore::Geometry* geometry, RHIMesh::ID& meshID, RHIMaterial::ID& materialID)
		{
			meshID = m_Meshes.size();
			materialID = m_Materials.size();

			m_Meshes.push_back(m_d3d12Private->CreateMesh(
				meshID,
				geometry->GetVertices(), geometry->GetVertexCount(), sizeof(RenderCore::Geometry::VertexType),
				geometry->GetIndices(), geometry->GetIndexCount(), sizeof(RenderCore::Geometry::IndexType)
			));

			m_Materials.push_back(m_d3d12Private->CreateMaterial(materialID));
		}

		void D3D12RHI::ExecuteCommandLists()
		{
			m_d3d12Private->ExecuteCommandLists();
		}

		void D3D12RHI::Flush()
		{
			m_d3d12Private->FlushCommandQueue();
		}

		void D3D12RHI::OnResize()
		{
			m_d3d12Private->OnResize();
		}
	}
}