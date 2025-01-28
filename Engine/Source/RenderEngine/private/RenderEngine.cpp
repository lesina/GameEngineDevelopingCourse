#include <Camera.h>
#include <Constants.h>
#include <RenderEngine.h>
#include <RenderObject.h>
#include <RHICore.h>
#include <RHIHelper.h>
#include <Material.h>
#include <RHIMesh.h>
#include <RHITexture.h>
#include <RHITechnique.h>
#include <RHIBuffer.h>
#include <RHIPipelineStateObject.h>

namespace GameEngine::Render
{
	// Supposed to be as a part of Render Pass
	struct ObjectConstants
	{
		Math::Matrix4x4f WorldViewProj = Math::Matrix4x4f::Identity();
	};

	struct MaterialConstants
	{
		Math::Vector4f Color = Math::Vector4f::Zero();
	};

	struct Vertex
	{
		Math::Vector3f Pos;
	};

	HAL::RHIBuffer::Ptr m_ObjectCB[RenderCore::g_FrameBufferCount];
	HAL::RHIBuffer::Ptr m_MaterialCB[RenderCore::g_FrameBufferCount];
	HAL::RHITechnique::Ptr m_Technique;
	HAL::RHIPipelineStateObject::Ptr m_PSO;
	HAL::RHITexture::Ptr depthStencil;
	std::vector<HAL::RHIMesh::Ptr> m_Meshes;
	std::vector<Material*> m_Materials;
	// End: Supposed to be as a part of Render Pass

	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("Vulkan");

		OnResize();

		for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
		{
			m_ObjectCB[i] = m_rhi->CreateBuffer(
				{
					.Count = RenderCore::g_MaximumRenderObjectCount,
					.ElementSize = sizeof(ObjectConstants),
					.UsageFlag = HAL::RHIBuffer::UsageFlag::ConstantBuffer
				}
			);

			m_MaterialCB[i] = m_rhi->CreateBuffer(
				{
					.Count = RenderCore::g_MaximumRenderObjectCount,
					.ElementSize = sizeof(MaterialConstants),
					.UsageFlag = HAL::RHIBuffer::UsageFlag::ConstantBuffer
				}
			);
		}

		HAL::RHITechnique::ShaderInfo shaderInfo =
		{
			{
				.Type = HAL::RHITechnique::ShaderInfoDescription::ShaderType::VertexShader,
				.ShaderFile = "Object.hlsl",
				.EntryPoint = "VS",
			},
			{
				.Type = HAL::RHITechnique::ShaderInfoDescription::ShaderType::PixelShader,
				.ShaderFile = "Object.hlsl",
				.EntryPoint = "PS",
			}
		};

		HAL::RHITechnique::InputLayout inputLayout =
		{
			{
				.SemanticName = "POSITION",
				.Index = 0,
				.Format = HAL::ResourceFormat::RGB32_FLOAT,
				.InputSlot = 0,
				.InputSlotClass = HAL::RHITechnique::InputLayoutDescription::Classification::PerVertex,
				.InstanceDataStepRate = 0
			}
		};

		HAL::RHITechnique::RootSignature rootSignature =
		{
			{
				.SlotIndex = 0,
				.SpaceIndex = 0,
				.IsConstantBuffer = true
			},
			{
				.SlotIndex = 1,
				.SpaceIndex = 0,
				.IsConstantBuffer = true
			}
		};

		m_Technique = m_rhi->CreateTechnique(shaderInfo, inputLayout, rootSignature);

		HAL::RHIPipelineStateObject::Description desc;
		desc.Technique = m_Technique;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetFormat();
		desc.DSVFormat = depthStencil->GetFormat();

		m_PSO = m_rhi->CreatePSO(desc);

		m_rhi->GetCommandList()->Close();
		m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

		m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
	}

	RenderEngine::~RenderEngine()
	{
	}

	void RenderEngine::Update(size_t frame)
	{
		m_rhi->GetCommandList()->GetAllocator()->Reset();

		m_rhi->GetCommandList()->Reset();

		OnResize();

		m_rhi->GetCommandList()->SetPipelineStateObject(m_PSO);

		HAL::Rect scissorRect(0, Core::g_MainWindowsApplication->GetWidth(), 0, Core::g_MainWindowsApplication->GetHeight());

		HAL::Viewport viewport(
			0.f, static_cast<float>(Core::g_MainWindowsApplication->GetWidth()),
			0.f, static_cast<float>(Core::g_MainWindowsApplication->GetHeight()),
			0.f, 1.f
		);

		m_rhi->GetCommandList()->SetViewport(viewport);
		m_rhi->GetCommandList()->SetScissorRect(scissorRect);

		m_rhi->GetCommandList()->ClearRenderTarget(m_rhi->GetSwapChain()->GetCurrentBackBuffer(), RenderCore::Colors::LightSteelBlue);
		m_rhi->GetCommandList()->ClearDepthStencilView(depthStencil, HAL::ClearFlags::Depth | HAL::ClearFlags::Stencil, 1.0f, 0);

		m_rhi->GetCommandList()->SetRenderTargets(1, m_rhi->GetSwapChain()->GetCurrentBackBuffer(), depthStencil);

		m_rhi->GetCommandList()->SetTechnique(m_Technique);

		for (RenderObject* renderObject : m_RenderObjects)
		{
			assert(renderObject);

			//Draw
			HAL::RHIMesh::ID meshID = renderObject->GetMeshID();
			Material::ID materialID = renderObject->GetMaterialID();

			assert(meshID >= 0);
			assert(meshID < m_Meshes.size());
			assert(meshID != RenderObject::k_invalidMeshID);
			assert(materialID >= 0);
			assert(materialID < m_Materials.size());
			assert(materialID != RenderObject::k_invalidMaterialID);

			// Projection and view matrices should be a part of Camera class
			Math::Matrix4x4f view = Core::g_MainCamera->GetViewMatrix();
			Math::Matrix4x4f proj = Math::ProjectionMatrixLH(0.25f * Math::Constants::PI, Core::g_MainWindowsApplication->GetAspectRatio(), 1.0f, 1000.0f);

			Math::Vector3f position = renderObject->GetPosition(frame);
			Math::Matrix4x4f world = Math::Matrix4x4f::Identity();
			world.SetElement(position.x, 3, 0);
			world.SetElement(position.y, 3, 1);
			world.SetElement(position.z, 3, 2);
			Math::Matrix4x4f worldViewProj = world * view * proj;

			ObjectConstants objConstants;
			objConstants.WorldViewProj = worldViewProj.Transpose();
			m_ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(meshID, &objConstants, sizeof(ObjectConstants));

			Material* material = m_Materials[materialID];
			MaterialConstants matConstants;
			matConstants.Color = material->GetAlbedo();
			m_MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(materialID, &matConstants, sizeof(MaterialConstants));

			m_rhi->GetCommandList()->SetMesh(m_Meshes[meshID]);
			m_rhi->GetCommandList()->SetPrimitiveTopology(HAL::PrimitiveTopology::TriangleList);

			m_rhi->GetCommandList()->SetGraphicsConstantBuffer(0, m_ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], meshID);
			m_rhi->GetCommandList()->SetGraphicsConstantBuffer(1, m_MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], materialID);

			m_rhi->GetCommandList()->DrawIndexedInstanced(
				m_Meshes[meshID]->GetIndexBuffer()->GetDesc().Count,
				1, 0, 0, 0);
		}

		m_rhi->GetSwapChain()->MakeBackBufferPresentable(m_rhi->GetCommandList());

		m_rhi->GetCommandList()->Close();
		m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

		m_rhi->GetSwapChain()->Present();

		m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
	}

	void RenderEngine::OnResize()
	{
		if (m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetWidth() != Core::g_MainWindowsApplication->GetWidth() ||
			m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetHeight() != Core::g_MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			assert(m_rhi->GetCommandList()->GetAllocator());

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());

			m_rhi->GetSwapChain()->Resize(
				m_rhi->GetDevice(),
				Core::g_MainWindowsApplication->GetWidth(),
				Core::g_MainWindowsApplication->GetHeight()
			);

			depthStencil.Reset();

			HAL::RHITexture::Description description;
			description.Dimension = HAL::RHITexture::Dimensions::Two;
			description.Width = Core::g_MainWindowsApplication->GetWidth();
			description.Height = Core::g_MainWindowsApplication->GetHeight();
			description.Format = HAL::ResourceFormat::D24S8;
			description.Flags = HAL::RHITexture::UsageFlags::DepthStencil;

			depthStencil = m_rhi->CreateTexture(description);

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
		}
	}

	void RenderEngine::CreateRenderObject(RenderCore::Geometry* geometry, RenderObject* renderObject)
	{
		assert(geometry);
		assert(renderObject);

		HAL::RHIMesh::ID meshID = m_Meshes.size();
		HAL::RHIMesh* mesh = m_rhi->CreateMesh(
			{
				.Count = geometry->GetVertexCount(),
				.ElementSize = sizeof(RenderCore::Geometry::VertexType),
				.initData = geometry->GetVertices()
			},
			{
				.Count = geometry->GetIndexCount(),
				.Format = HAL::ResourceFormat::R16_UINT,
				.initData = geometry->GetIndices()
			});

		m_Meshes.push_back(mesh);

		Material::ID materialID = m_Materials.size();
		Material* material = new Material(materialID);
		m_Materials.push_back(material);

		renderObject->SetMeshID(meshID);
		renderObject->SetMaterialID(materialID);
		m_RenderObjects.push_back(renderObject);
	}
}