#include <RenderEngine.h>
#include <RHI.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
	}

	void RenderEngine::Update()
	{
		m_rhi->Test();
	}
}