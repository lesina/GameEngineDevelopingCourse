#pragma once

#include <d3d12.h>

#include <RHIDevice.h>
#include <D3D12RHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIDevice final : public RHIDevice
		{
		public:
			using Ptr = RefCountPtr<D3D12RHIDevice>;

		public:
			D3D12RHIDevice() = delete;
			D3D12RHIDevice(D3D12RHIFactory::Ptr factory);
			~D3D12RHIDevice() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Device> GetHandle() const;

		private:
			RefCountPtr<ID3D12Device> m_NativeDevice = nullptr;
		};
	}
}