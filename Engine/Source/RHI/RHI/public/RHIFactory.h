#pragma once

#include <RHICommon.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHIFactory : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHIFactory>;
		};
	}
}