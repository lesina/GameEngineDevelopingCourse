#include <RenderEngine.h>
#include <RenderData.h>
#include <RenderObject.h>
#include <RHIHelper.h>
#include <RHIMaterial.h>
#include <RHIMesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		m_rhi->Init();
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update(size_t frame)
	{
		OnResize();

		m_rhi->BeginFrame();

		for (RenderObject* renderObject : m_RenderObjects)
		{
			m_rhi->Draw(renderObject->GetRenderData(), frame);
		}

		m_rhi->EndFrame();

		m_rhi->Flush();
	}

	void RenderEngine::OnResize()
	{
		if (m_swapChainWidth != Core::g_MainWindowsApplication->GetWidth() ||
			m_swapChainHeight != Core::g_MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			m_swapChainWidth = Core::g_MainWindowsApplication->GetWidth();
			m_swapChainHeight = Core::g_MainWindowsApplication->GetHeight();
			m_rhi->OnResize();
		}
	}

	void RenderEngine::CreateRenderObject(RenderCore::Geometry::Ptr geometry, RenderObject** renderObject)
	{
		assert(geometry);
		assert(renderObject);

		HAL::RHIMesh::ID meshID = HAL::RHIMesh::k_invalidMeshID;
		HAL::RHIMaterial::ID materialID = HAL::RHIMaterial::k_invalidMaterialID;
		m_rhi->CreateMesh(geometry, meshID, materialID);

		HAL::RenderData* renderData = new HAL::RenderData(meshID, materialID);

		*renderObject = new RenderObject(renderData);

		m_RenderObjects.push_back(*renderObject);
	}
}