#pragma once

#include <RenderCore/export.h>

namespace GameEngine
{
	namespace RenderCore
	{
		// More than 2 frames is not officialy supported
		// All the code logic is made in the way so triple buffering should work
		inline constexpr size_t g_FrameBufferCount = 2;

		// Since we require to allocate memory for GPU heap resources
		// Let's assume that we have no more than 512 static meshes at a time
		inline constexpr size_t g_MaximumRenderObjectCount = 512;

		// Power of 2
		inline constexpr size_t g_MaximumRenderTargets = 32;
		inline constexpr size_t g_MaximumDepthStencilTargets = g_MaximumRenderTargets / 2;
		inline constexpr size_t g_MaximumCBV_SRV_UAV = g_MaximumRenderObjectCount * 5 + g_MaximumRenderTargets;
		inline constexpr size_t g_MaximumSamplers = 16;

		// There will be all the rendering settings we need
		struct RenderSettings
		{
			
		};

		extern RENDER_CORE_API std::unique_ptr<RenderSettings> g_RenderSettings;
	}
}