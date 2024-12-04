#pragma once

#include <d3d12.h>

#include <RHICommandQueue.h>
#include <D3D12RHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHICommandQueue final : public RHICommandQueue
		{
		public:
			using Ptr = RefCountPtr<D3D12RHICommandQueue>;

		public:
			D3D12RHICommandQueue() = delete;
			D3D12RHICommandQueue(D3D12RHIDevice::Ptr device);
			~D3D12RHICommandQueue() = default;

		public:
			virtual void ExecuteCommandLists(const std::vector<RHICommandList::Ptr>& cmdLists) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12CommandQueue> GetHandle() const;

		private:
			RefCountPtr<ID3D12CommandQueue> m_NativeCommandQueue = nullptr;
		};
	}
}