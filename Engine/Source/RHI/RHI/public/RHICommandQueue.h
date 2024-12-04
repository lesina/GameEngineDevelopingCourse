#pragma once

#include <RHICommon.h>
#include <RHICommandList.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHICommandQueue : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHICommandQueue>;

		public:
			virtual void ExecuteCommandLists(const std::vector<RHICommandList::Ptr>& cmdLists) = 0;
		};
	}
}