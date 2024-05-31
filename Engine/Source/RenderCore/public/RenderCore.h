#pragma once

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
	}
}