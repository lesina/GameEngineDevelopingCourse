#pragma once

#include <RenderEngine/export.h>
#include <RenderObject.h>
#include <Window/IWindow.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHIAdapter;
	}

	class RENDER_ENGINE_API RenderEngine final
	{
	public:
		RenderEngine();

		void Update();
        void MoveTetrahedron();
		void OnResize(uint16_t width, uint16_t height);

		inline uint16_t GetSwapChainWidth() const { return m_swapChainWidth; }
		inline uint16_t GetSwapChainHeight() const { return m_swapChainHeight; }

	private:
		uint16_t m_swapChainWidth;
		uint16_t m_swapChainHeight;

		std::vector<RenderObject::Ptr> m_RenderObjects;

		std::shared_ptr<HAL::RHIAdapter> m_rhi;
	};
}