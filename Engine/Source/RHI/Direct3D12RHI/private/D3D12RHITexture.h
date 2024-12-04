#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <RHITexture.h>
#include <D3D12RHISwapChain.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHITexture final : public RHITexture
		{
		public:
			using Ptr = RefCountPtr<D3D12RHITexture>;

		public:
			D3D12RHITexture() = delete;
			D3D12RHITexture(
				const RHITexture::Description& desc,
				RefCountPtr<ID3D12Resource> resource,
				D3D12_CPU_DESCRIPTOR_HANDLE srv,
				D3D12_CPU_DESCRIPTOR_HANDLE rtv,
				D3D12_CPU_DESCRIPTOR_HANDLE dsv
			);

		public:
			D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE GetShaderResourceView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Resource> GetHandle() const;
			ID3D12Resource** GetHandleAddress();

			D3D12_RESOURCE_STATES GetCurrentState() const;
			
			/*
			* We can safely make it public since it is just a pure d3d12 implememntation and it will be visible only
			* within the D3D12RHI module. It still can cause us problems, but
			* the other solution would be making a cmd list a friend class
			*/
			void SetCurrentState(D3D12_RESOURCE_STATES state);

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE m_RenderTargetView;
			D3D12_CPU_DESCRIPTOR_HANDLE m_ShaderResourceView;
			D3D12_CPU_DESCRIPTOR_HANDLE m_DepthStencilView;

			RefCountPtr<ID3D12Resource> m_NativeResource = nullptr;

			D3D12_RESOURCE_STATES m_CurrentState = D3D12_RESOURCE_STATE_COMMON;
		};
	}
}