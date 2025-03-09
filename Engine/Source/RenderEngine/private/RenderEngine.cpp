#include <Camera.h>
#include <Constants.h>
#include <RenderEngine.h>
#include <RenderObject.h>
#include <RHICore.h>
#include <RHIHelper.h>
#include <Material.h>
#include <Mesh.h>
#include <Texture.h>
#include <Technique.h>
#include <Buffer.h>
#include <PipelineStateObject.h>
#include <FileSystem.h>
#include <RenderPipeline/RenderPipeline.h>

#include <GUIContext.h>

namespace cqe::Render
{
	RenderEngine::RenderEngine()
	{
		assert(RenderThread::IsRenderThread());

		m_rhi = RHI::Helper::CreateRHI("D3D12");

		if (bool minimalRequirementsMet = m_rhi->CheckMinimalRequirements(); !minimalRequirementsMet)
		{
			ASSERT_NOT_IMPLEMENTED;
		}

		GUI::GUIContext::GetInstance()->InitRenderBackend(m_rhi);

		// Next time we reallocate the vector - Render Pipeline explodes
		// This issue should be resolved by reworking the RenderPipeline structure
		m_RenderObjects.reserve(1024);
		m_RenderPipeline = new RenderPipeline(m_RenderObjects, m_rhi);
	}

	RenderEngine::~RenderEngine()
	{
		for (RenderObject* renderObject : m_RenderObjects)
		{
			m_rhi->FreeTexture(renderObject->GetMaterial()->GetTexture(), RHI::Texture::UsageFlags::ShaderResource);
			
			delete renderObject;
		}

		delete m_RenderPipeline;

		GUI::GUIContext::GetInstance()->DeinitRenderBackend(m_rhi);
	}

	void RenderEngine::Update()
	{
		assert(RenderThread::IsRenderThread());

		m_RenderPipeline->Render();
	}

	void RenderEngine::OnResize()
	{
		assert(RenderThread::IsRenderThread());

		m_RenderPipeline->Resize();
	}

	void RenderEngine::CreateRenderObject(RenderCore::Geometry* geometry, RenderObject* renderObject)
	{
		assert(geometry);
		assert(renderObject);
		assert(RenderThread::IsRenderThread());

		RHI::Mesh* mesh = m_rhi->CreateMesh(
			{
				.Count = geometry->GetVertexCount(),
				.ElementSize = sizeof(RenderCore::Geometry::VertexType),
				.initData = geometry->GetVertices()
			},
			{
				.Count = geometry->GetIndexCount(),
				.Format = RHI::ResourceFormat::R16_UINT,
				.initData = geometry->GetIndices()
			});

		RHI::Texture* texture = m_rhi->CreateTexture(
			{
				// TODO: fetch these values from somewhere
				.Dimension = RHI::Texture::Dimensions::Two,
				.Width = 512,
				.Height = 512,
				.MipLevels = 10,
				.Format = RHI::ResourceFormat::BC1_UNORM,
				.Flags = RHI::Texture::UsageFlags::ShaderResource
			}
		);

		m_rhi->LoadTextureFromFile(texture, Core::g_FileSystem->GetFilePath("Texture.dds"));

		Material* material = new Material(texture);

		renderObject->SetMesh(mesh);
		renderObject->SetMaterial(material);
		m_RenderObjects.push_back(renderObject);

		// We will deal with this later. If we are creating more than 1024 renderobjects, we are reallocating the vector
		// So the vector inside of RenderPipeline becomes invalid
		// Put an assert here to not forget this problem
		assert(m_RenderObjects.size() < 1024);
	}
}