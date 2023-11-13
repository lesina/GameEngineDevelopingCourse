#pragma once

#include <RHICommon.h>
#include <RHI/RHI/export.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHI_API RHIHelper final
		{
		public:
			static RHI::Ptr CreateRHI(const std::string& RHIName);
		};
	}
}