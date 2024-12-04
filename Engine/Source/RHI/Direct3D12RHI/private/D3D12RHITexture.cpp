#include <D3D12RHITexture.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHITexture::D3D12RHITexture(
			const RHITexture::Description& desc,
			RefCountPtr<ID3D12Resource> resource,
			D3D12_CPU_DESCRIPTOR_HANDLE srv,
			D3D12_CPU_DESCRIPTOR_HANDLE rtv,
			D3D12_CPU_DESCRIPTOR_HANDLE dsv
		)
			: RHITexture(desc),
			m_NativeResource(resource),
			m_RenderTargetView(srv),
			m_ShaderResourceView(rtv),
			m_DepthStencilView(dsv)
		{

		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12RHITexture::GetRenderTargetView() const
		{
			assert((m_Description.Flags & UsageFlags::RenderTarget) != 0);
			return m_RenderTargetView;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12RHITexture::GetShaderResourceView() const
		{
			assert((m_Description.Flags & UsageFlags::ShaderResource) != 0);
			return m_ShaderResourceView;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12RHITexture::GetDepthStencilView() const
		{
			assert((m_Description.Flags & UsageFlags::DepthStencil) != 0);
			return m_DepthStencilView;
		}

		RenderNativeObject D3D12RHITexture::GetNativeObject()
		{
			return RenderNativeObject(m_NativeResource.Get());
		}

		RefCountPtr<ID3D12Resource> D3D12RHITexture::GetHandle() const
		{
			return m_NativeResource;
		}

		ID3D12Resource** D3D12RHITexture::GetHandleAddress()
		{
			return &m_NativeResource;
		}

		D3D12_RESOURCE_STATES D3D12RHITexture::GetCurrentState() const
		{
			return m_CurrentState;
		}

		void D3D12RHITexture::SetCurrentState(D3D12_RESOURCE_STATES state)
		{
			m_CurrentState = state;
		}
	}
}