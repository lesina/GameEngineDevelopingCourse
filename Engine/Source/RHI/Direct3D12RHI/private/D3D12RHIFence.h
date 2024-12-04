#pragma once

#include <d3d12.h>

#include <RHIFence.h>
#include <D3D12RHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIFence final : public RHIFence
		{
		public:
			using Ptr = RefCountPtr<D3D12RHIFence>;

		public:
			D3D12RHIFence() = delete;
			D3D12RHIFence(D3D12RHIDevice::Ptr device);
			~D3D12RHIFence() = default;

		public:
			virtual void Sync(RHICommandQueue::Ptr commandQueue) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Fence> GetHandle() const;

		private:
			uint64_t m_CurrentFence = 0;

			RefCountPtr<ID3D12Fence> m_NativeFence = nullptr;
		};
	}
}