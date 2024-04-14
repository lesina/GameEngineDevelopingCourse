#pragma once

#include <RHIAdapter.h>
#include <RHI/RHI/export.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHI_API RHIHelper final
		{
		public:
			// RHI - "Vulkan" or "D3D12"
			static RHIAdapter::Ptr CreateRHI(const std::string& RHIName);
		};
	}
}