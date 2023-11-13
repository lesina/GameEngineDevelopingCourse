#pragma once

#include <RenderEngine/export.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHI;
	}

	class RENDER_ENGINE_API RenderEngine final
	{
	public:
		RenderEngine();

		void Update();

	private:
		std::shared_ptr<HAL::RHI> m_rhi;
	};
}