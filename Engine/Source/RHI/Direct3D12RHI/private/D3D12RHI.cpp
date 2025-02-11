#include <array.h>
#include <D3D12Mesh.h>
#include <D3D12RHI.h>
#include <D3D12RHIPrivate.h>
#include <D3D12Material.h>
#include <Math/Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHI::D3D12RHI() :
			m_d3d12Private(std::make_shared<D3D12RHIPrivate>())
		{
			m_d3d12Private->EnableDebugLayer();
		}

		void D3D12RHI::Init()
		{
			m_d3d12Private->Init();
		}

		void D3D12RHI::Update(Mesh::Ptr mesh, Material::Ptr material)
		{
			m_d3d12Private->Update(mesh, material);
		}

		Mesh::Ptr D3D12RHI::CreateBoxMesh()
		{
			array<Vertex, 8> vertices =
			{
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Green) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Cyan) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Magenta) })
			};

			array<uint16_t, 36> indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3,
				4, 3, 7
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}

        Mesh::Ptr D3D12RHI::CreateTetrahedronMesh(Math::Vector3f pos)
		{
			array<Vertex, 5> vertices =
			{
				Vertex({ pos - -Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ pos - -Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Aquamarine) }),
				Vertex({ pos - -Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::DeepSkyBlue) }),
				Vertex({ pos - -Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::DarkSlateBlue) }),
				Vertex({ pos - -Math::Vector3f( 0.0f, +1.0f,  0.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
			};

			array<uint16_t, 18> indices =
			{
                // base
                2, 0, 1,
                2, 3, 0,
                // sides
				0, 1, 4,
				1, 2, 4,
				2, 3, 4,
				3, 0, 4,
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}

		Material::Ptr D3D12RHI::GetMaterial(const std::string& name)
		{
			return m_d3d12Private->GetMaterial(name);
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