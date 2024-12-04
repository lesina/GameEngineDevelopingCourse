#pragma once

#include <RHICommon.h>
#include <RHICommandList.h>
#include <RHIDevice.h>
#include <RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHISwapChain : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<RHISwapChain>;

		public:
			virtual void Present() = 0;
			virtual void MakeBackBufferPresentable(RHICommandList::Ptr commandList) = 0;

		public:
			virtual void Resize(RHIDevice::Ptr device, uint32_t width, uint32_t height) = 0;
			virtual RHITexture::Ptr GetCurrentBackBuffer() = 0;

			uint64_t GetCurrentBackBufferIdx() const { return m_CurrentBackBufferIdx; }

		protected:
			uint64_t m_CurrentBackBufferIdx = 0;
		};
	}
}