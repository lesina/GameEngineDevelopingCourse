#include <RenderPipeline/Features/OpaqueForward/Passes/ForwardPass.h>

#include <Window/IWindow.h>
#include <Camera.h>
#include <Constants.h>

namespace cqe
{
	namespace Render
	{
		ForwardPass::ForwardPass(
			const std::vector<RenderObject*>& renderObjects,
			RHI::Context* rhi
		) : RenderPass(rhi),
			m_RenderObjects(renderObjects)
		{
			assert(rhi);

			Resize();

			for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
			{
				m_ObjectCB[i] = m_rhi->CreateBuffer(
					{
						.Count = RenderCore::g_MaximumRenderObjectCount,
						.ElementSize = sizeof(ObjectConstants),
						.UsageFlag = RHI::Buffer::UsageFlag::ConstantBuffer
					}
				);

				m_MaterialCB[i] = m_rhi->CreateBuffer(
					{
						.Count = RenderCore::g_MaximumRenderObjectCount,
						.ElementSize = sizeof(MaterialConstants),
						.UsageFlag = RHI::Buffer::UsageFlag::ConstantBuffer
					}
				);
			}

			RHI::Technique::ShaderInfo shaderInfo =
			{
				{
					.Type = RHI::Technique::ShaderInfoDescription::ShaderType::VertexShader,
					.ShaderFile = "Object.hlsl",
					.EntryPoint = "VS",
				},
				{
					.Type = RHI::Technique::ShaderInfoDescription::ShaderType::PixelShader,
					.ShaderFile = "Object.hlsl",
					.EntryPoint = "PS",
				}
			};

			RHI::Technique::InputLayout inputLayout =
			{
				{
					.SemanticName = "POSITION",
					.Index = 0,
					.Format = RHI::ResourceFormat::RGB32_FLOAT,
					.InputSlot = 0,
					.InputSlotClass = RHI::Technique::InputLayoutDescription::Classification::PerVertex,
					.InstanceDataStepRate = 0
				},
				{
					.SemanticName = "TEXCOORD",
					.Index = 0,
					.Format = RHI::ResourceFormat::RG32_FLOAT,
					.InputSlot = 0,
					.InputSlotClass = RHI::Technique::InputLayoutDescription::Classification::PerVertex,
					.InstanceDataStepRate = 0
				}
			};

			RHI::Technique::RootSignatureDescription rootSignatureDescription =
			{
				.Type = "BasicRootSignature"
			};

			m_Technique = m_rhi->CreateTechnique(shaderInfo, inputLayout, rootSignatureDescription);

			RHI::PipelineStateObject::Description desc;
			desc.Technique = m_Technique;
			desc.NumRenderTargets = 1;
			desc.RTVFormats[0] = m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetFormat();
			desc.DSVFormat = m_DepthStencil->GetFormat();

			m_PSO = m_rhi->CreatePSO(desc);

			m_rhi->GetCommandList()->Close();
			m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
		}

		void ForwardPass::Render()
		{
			m_rhi->GetCommandList()->GetAllocator()->Reset();

			m_rhi->GetCommandList()->Reset();

			m_rhi->SetDescriptorHeaps();

			m_rhi->GetCommandList()->SetPipelineStateObject(m_PSO);

			RHI::Rect scissorRect(0, Core::g_MainWindowsApplication->GetWidth(), 0, Core::g_MainWindowsApplication->GetHeight());

			RHI::Viewport viewport(
				0.f, static_cast<float>(Core::g_MainWindowsApplication->GetWidth()),
				0.f, static_cast<float>(Core::g_MainWindowsApplication->GetHeight()),
				0.f, 1.f
			);

			m_rhi->GetCommandList()->SetViewport(viewport);
			m_rhi->GetCommandList()->SetScissorRect(scissorRect);

			m_rhi->GetCommandList()->ClearRenderTarget(m_rhi->GetSwapChain()->GetCurrentBackBuffer(), RenderCore::Colors::LightSteelBlue);
			m_rhi->GetCommandList()->ClearDepthStencilView(m_DepthStencil, RHI::ClearFlags::Depth | RHI::ClearFlags::Stencil, 1.0f, 0);

			m_rhi->GetCommandList()->SetRenderTargets(1, m_rhi->GetSwapChain()->GetCurrentBackBuffer(), m_DepthStencil);

			m_rhi->GetCommandList()->SetTechnique(m_Technique);

			for (RenderObject* renderObject : m_RenderObjects)
			{
				assert(renderObject);

				//Draw
				const RHI::Mesh* mesh = renderObject->GetMesh();
				const Material* material = renderObject->GetMaterial();

				assert(mesh != nullptr);
				assert(material != nullptr);

				// Projection and view matrices should be a part of Camera class
				Math::Matrix4x4f view = Core::g_MainCamera->GetViewMatrix();
				Math::Matrix4x4f proj = Math::ProjectionMatrixLH(0.25f * Math::Constants::PI, Core::g_MainWindowsApplication->GetAspectRatio(), 1.0f, 1000.0f);

				Math::Vector3f position = renderObject->GetPosition();
				Math::Matrix4x4f world = Math::Matrix4x4f::Identity();
				world.SetElement(position.x, 3, 0);
				world.SetElement(position.y, 3, 1);
				world.SetElement(position.z, 3, 2);
				Math::Matrix4x4f worldViewProj = world * view * proj;

				ObjectConstants objConstants;
				objConstants.WorldViewProj = worldViewProj.Transpose();
				m_ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(mesh->GetID(), &objConstants, sizeof(ObjectConstants));

				MaterialConstants matConstants;
				matConstants.Color = material->GetAlbedo();
				m_MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(material->GetID(), &matConstants, sizeof(MaterialConstants));

				m_rhi->GetCommandList()->SetMesh(mesh);
				m_rhi->GetCommandList()->SetPrimitiveTopology(RHI::PrimitiveTopology::TriangleList);

				m_rhi->GetCommandList()->SetGraphicsConstantBuffer(0, m_ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], mesh->GetID());
				m_rhi->GetCommandList()->SetGraphicsConstantBuffer(1, m_MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], material->GetID());

				m_rhi->GetCommandList()->SetGraphicsDescriptorTableForTexture(2, material->GetTexture());

				m_rhi->GetCommandList()->DrawIndexedInstanced(
					mesh->GetIndexBuffer()->GetDesc().Count,
					1, 0, 0, 0);
			}
		}

		void ForwardPass::Resize()
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

				m_DepthStencil.Reset();

				RHI::Texture::Description description;
				description.Dimension = RHI::Texture::Dimensions::Two;
				description.Width = Core::g_MainWindowsApplication->GetWidth();
				description.Height = Core::g_MainWindowsApplication->GetHeight();
				description.Format = RHI::ResourceFormat::D24S8;
				description.Flags = RHI::Texture::UsageFlags::DepthStencil;

				m_DepthStencil = m_rhi->CreateTexture(description);

				m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
			}
		}
	}
}