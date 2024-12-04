#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <D3D12RHICommandQueue.h>
#include <D3D12RHIDevice.h>
#include <D3D12RHIFactory.h>
#include <RHISwapChain.h>

#include <RenderCore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHISwapChain final : public RHISwapChain
		{
		public:
			using Ptr = RefCountPtr<D3D12RHISwapChain>;

		public:
			D3D12RHISwapChain() = delete;
			D3D12RHISwapChain(
				D3D12RHIDevice::Ptr device,
				D3D12RHIFactory::Ptr factory,
				D3D12RHICommandQueue::Ptr commandQueue
			);
			~D3D12RHISwapChain() = default;

		public:
			virtual void Present() override;
			virtual void MakeBackBufferPresentable(RHICommandList::Ptr commandList) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<IDXGISwapChain> GetHandle() const;

		public:
			virtual void Resize(RHIDevice::Ptr device, uint32_t width, uint32_t height) override;
			virtual RHITexture::Ptr GetCurrentBackBuffer() override;

		private:
			RHITexture::Ptr m_BackBuffer[RenderCore::g_FrameBufferCount];
			RefCountPtr<IDXGISwapChain> m_NativeSwapChain = nullptr;
			RefCountPtr<ID3D12DescriptorHeap> m_BackBufferHeap = nullptr;
			DXGI_FORMAT m_BackBufferFormat;
		};
	}
}