#pragma once

#include <RHICommon.h>
#include <RHICommandQueue.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHIFence : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHIFence>;

		public:
			virtual void Sync(RHICommandQueue::Ptr commandQueue) = 0;
		};
	}
}