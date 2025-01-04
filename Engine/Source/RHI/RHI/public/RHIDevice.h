#pragma once

#include <RHICommon.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHIDevice : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHIDevice>;
		};
	}
}