#include <RenderPipeline/RenderPipeline.h>

#include <RenderPipeline/Features/OpaqueForward/OpaqueForwardFeature.h>

#include <GUIContext.h>

namespace cqe
{
	namespace Render
	{
		/*
		* Better name for this class is GraphicsPipeline.
		* But to avoid confusion with copy, compute and graphics commands, the decision was made towards RenderPipeline.
		* The whole idea of this abstraction is to incapsulate the whole rhi logic from the render engine.
		* 
		* While render engine is still responsible for water calculations, octree occlusions, preparing data,
		* this class will be responsible for the common GPU pipeline
		*/
		RenderPipeline::RenderPipeline(
			const std::vector<RenderObject*>& renderObjects,
			std::shared_ptr<RHI::Context> rhi
		) : m_rhi(rhi.get())
		{
			assert(m_rhi);

			// Allocating new features separately from setting them into the array because we are going to set all the dependencies between features manually
			OpaqueForwardFeature* opaqueForwardFeature = new OpaqueForwardFeature(renderObjects, m_rhi);

			m_Features =
			{
				opaqueForwardFeature
			};
		}

		RenderPipeline::~RenderPipeline()
		{
			for (RenderFeature* renderFeature : m_Features)
			{
				delete renderFeature;
			}
		}

		void RenderPipeline::Render()
		{
			for (RenderFeature* renderFeature : m_Features)
			{
				renderFeature->Render();
			}

			GUI::GUIContext::GetInstance()->Render();

			m_rhi->GetSwapChain()->MakeBackBufferPresentable(m_rhi->GetCommandList());

			m_rhi->GetCommandList()->Close();
			m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

			m_rhi->GetSwapChain()->Present();

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
		}

		void RenderPipeline::Resize()
		{
			for (RenderFeature* renderFeature : m_Features)
			{
				renderFeature->Resize();
			}
		}
	}
}