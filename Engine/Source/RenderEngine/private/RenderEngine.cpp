#include <RenderEngine.h>
#include <RHIHelper.h>
#include <Mesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		m_rhi->Init();

		RenderObject::Ptr test_object = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(test_object);
		//test_object->m_mesh = m_rhi->CreateBoxMesh();
		test_object->m_mesh = m_rhi->CreateTetrahedronMesh(Core::Math::Vector3f::Zero());
		test_object->m_material = m_rhi->GetMaterial(test_object->m_mesh->GetName());
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update()
	{
        MoveTetrahedron();
		m_rhi->Update(m_RenderObjects[0]->m_mesh, m_RenderObjects[0]->m_material);
		m_rhi->Flush();
	}

    void RenderEngine::MoveTetrahedron()
    {
        static float z_pos = 0;
        z_pos += 0.1;
        RenderObject::Ptr tetrahedron = m_RenderObjects[0];
        tetrahedron->m_mesh = m_rhi->CreateTetrahedronMesh(Core::Math::Vector3f(0, 0, 0));
        tetrahedron->m_material = m_rhi->GetMaterial(tetrahedron->m_mesh->GetName());
    }

	void RenderEngine::OnResize(uint16_t width, uint16_t height)
	{
		m_swapChainWidth = width;
		m_swapChainHeight = height;
		m_rhi->OnResize();
	}
}