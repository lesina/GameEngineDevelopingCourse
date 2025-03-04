#include <D3D12Texture.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Texture::D3D12Texture(
			const Texture::Description& desc,
			RefCountPtr<ID3D12Resource> resource,
			D3D12_CPU_DESCRIPTOR_HANDLE rtv,
			D3D12_CPU_DESCRIPTOR_HANDLE srv,
			D3D12_CPU_DESCRIPTOR_HANDLE dsv,
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpu
		)
			: Texture(desc),
			m_NativeResource(resource),
			m_RenderTargetView(rtv),
			m_ShaderResourceView(srv),
			m_DepthStencilView(dsv),
			m_GpuShaderResourceView(srvGpu)
		{

		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture::GetRenderTargetView() const
		{
			assert((m_Description.Flags & UsageFlags::RenderTarget) != 0);
			return m_RenderTargetView;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture::GetShaderResourceView() const
		{
			assert((m_Description.Flags & UsageFlags::ShaderResource) != 0);
			return m_ShaderResourceView;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture::GetDepthStencilView() const
		{
			assert((m_Description.Flags & UsageFlags::DepthStencil) != 0);
			return m_DepthStencilView;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE D3D12Texture::GetGpuShaderResourceView() const
		{
			assert((m_Description.Flags & UsageFlags::ShaderResource) != 0);
			return m_GpuShaderResourceView;
		}

		RenderNativeObject D3D12Texture::GetNativeObject()
		{
			return RenderNativeObject(m_NativeResource.Get());
		}

		RefCountPtr<ID3D12Resource> D3D12Texture::GetHandle() const
		{
			return m_NativeResource;
		}

		ID3D12Resource** D3D12Texture::GetHandleAddress()
		{
			return &m_NativeResource;
		}

		D3D12_RESOURCE_STATES D3D12Texture::GetCurrentState() const
		{
			return m_CurrentState;
		}

		void D3D12Texture::SetCurrentState(D3D12_RESOURCE_STATES state)
		{
			m_CurrentState = state;
		}
	}
}