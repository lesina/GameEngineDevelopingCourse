#pragma once

#include <RHICommon.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHICommandAllocator : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHICommandAllocator>;

		public:
			virtual void Reset() = 0;
		};
	}
}