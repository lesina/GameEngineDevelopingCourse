#pragma once

#include <d3d12.h>

#include <RHICommandAllocator.h>
#include <D3D12RHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHICommandAllocator final : public RHICommandAllocator
		{
		public:
			using Ptr = RefCountPtr<D3D12RHICommandAllocator>;

		public:
			D3D12RHICommandAllocator() = delete;
			D3D12RHICommandAllocator(D3D12RHIDevice::Ptr device);
			~D3D12RHICommandAllocator() = default;

		public:
			virtual void Reset() override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12CommandAllocator> GetHandle() const;

		private:
			RefCountPtr<ID3D12CommandAllocator> m_NativeCommandAllocator = nullptr;
		};
	}
}