#include <RenderEngine.h>
#include <RHIHelper.h>
#include <Mesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine(Core::Window::Ptr window)
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		SetMainWindow(window);
		m_rhi->Init();

		RenderObject::Ptr box = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(box);
		box->m_mesh = m_rhi->CreateBoxMesh();
		box->m_material = m_rhi->GetMaterial(box->m_mesh->GetName());
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update()
	{
		m_rhi->Update(m_RenderObjects[0]->m_mesh, m_RenderObjects[0]->m_material);

		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		m_rhi->Flush();
	}

	void RenderEngine::SetMainWindow(Core::Window::Ptr window)
	{
		m_rhi->SetMainWindow(window);
	}

	void RenderEngine::OnResize()
	{
		m_rhi->OnResize();
	}
}