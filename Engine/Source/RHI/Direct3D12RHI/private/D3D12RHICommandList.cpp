#include <D3D12RHIBuffer.h>
#include <D3D12RHICore.h>
#include <D3D12RHICommandList.h>
#include <D3D12RHICommandAllocator.h>
#include <D3D12RHIPipelineStateObject.h>
#include <D3D12RHITechnique.h>
#include <D3D12RHITexture.h>
#include <RHIMesh.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHICommandList::D3D12RHICommandList(D3D12RHIDevice::Ptr device)
		{
			assert(device != nullptr);
			
			m_CommandAllocator = new D3D12RHICommandAllocator(device);

			RefCountPtr<ID3D12CommandAllocator> d3d12CmdAllocator = m_CommandAllocator->GetHandle();

			HRESULT hr = device->GetHandle()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				d3d12CmdAllocator.Get(),
				nullptr,
				IID_PPV_ARGS(m_NativeCommandList.GetAddressOf()));
			assert(SUCCEEDED(hr));

			Close();
		}

		void D3D12RHICommandList::ClearRenderTarget(RHITexture::Ptr renderTarget, RenderCore::Color& color)
		{
			assert(renderTarget != nullptr);

			float colorRGBA[4] = { color.x, color.y, color.z, color.w };

			D3D12RHITexture* d3d12renderTarget = reinterpret_cast<D3D12RHITexture*>(renderTarget.Get());

			ValidateTextureState(d3d12renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);

			m_NativeCommandList->ClearRenderTargetView(d3d12renderTarget->GetRenderTargetView(), colorRGBA, 0, nullptr);
		}

		void D3D12RHICommandList::ClearDepthStencilView(RHITexture::Ptr depthStencil, ClearFlags::Flag clearFlags, float depth, uint8_t stencil)
		{
			assert(depthStencil != nullptr);

			D3D12RHITexture* d3d12depthStencil = reinterpret_cast<D3D12RHITexture*>(depthStencil.Get());

			ValidateTextureState(d3d12depthStencil, D3D12_RESOURCE_STATE_DEPTH_WRITE);

			m_NativeCommandList->ClearDepthStencilView(d3d12depthStencil->GetDepthStencilView(), ConvertToD3D12ClearFlag(clearFlags), depth, stencil, 0, nullptr);
		}

		void D3D12RHICommandList::SetRenderTargets(uint32_t TargetsNum, RHITexture::Ptr renderTarget, RHITexture::Ptr depthStencil)
		{
			assert(renderTarget != nullptr);
			assert(depthStencil != nullptr);
			assert(TargetsNum == 1); // Do not support more targets at the moment

			D3D12RHITexture* d3d12renderTarget = reinterpret_cast<D3D12RHITexture*>(renderTarget.Get());
			D3D12RHITexture* d3d12depthStencil = reinterpret_cast<D3D12RHITexture*>(depthStencil.Get());

			ValidateTextureState(d3d12renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET);
			ValidateTextureState(d3d12depthStencil, D3D12_RESOURCE_STATE_DEPTH_WRITE);

			D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = d3d12renderTarget->GetRenderTargetView();
			D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = d3d12depthStencil->GetDepthStencilView();

			m_NativeCommandList->OMSetRenderTargets(TargetsNum, &renderTargetView, true, &depthStencilView);
		}

		void D3D12RHICommandList::SetViewport(const Viewport& viewport)
		{
			D3D12_VIEWPORT d3d12viewport;
			d3d12viewport.TopLeftX = viewport.LeftX;
			d3d12viewport.TopLeftY = viewport.TopY;
			d3d12viewport.Width = viewport.GetWidth();
			d3d12viewport.Height = viewport.GetHeight();
			d3d12viewport.MinDepth = viewport.MinDepth;
			d3d12viewport.MaxDepth = viewport.MaxDepth;

			m_NativeCommandList->RSSetViewports(1, &d3d12viewport);
		}

		void D3D12RHICommandList::SetScissorRect(const Rect& scissorRect)
		{
			D3D12_RECT d3d12ScissorRect;
			d3d12ScissorRect.left = scissorRect.LeftX;
			d3d12ScissorRect.right = scissorRect.RightX;
			d3d12ScissorRect.top = scissorRect.TopY;
			d3d12ScissorRect.bottom = scissorRect.BottomY;

			m_NativeCommandList->RSSetScissorRects(1, &d3d12ScissorRect);
		}

		RHICommandAllocator::Ptr D3D12RHICommandList::GetAllocator() const
		{
			return m_CommandAllocator;
		}

		void D3D12RHICommandList::Close()
		{
			HRESULT hr = m_NativeCommandList->Close();
			assert(SUCCEEDED(hr));
		}

		void D3D12RHICommandList::Reset()
		{
			ID3D12CommandAllocator* d3d12CmdAllocator = (ID3D12CommandAllocator*)(m_CommandAllocator->GetNativeObject());

			HRESULT hr = m_NativeCommandList->Reset(d3d12CmdAllocator, nullptr);
			assert(SUCCEEDED(hr));
		}

		void D3D12RHICommandList::SetPipelineStateObject(RHIPipelineStateObject::Ptr pso)
		{
			assert(pso != nullptr);

			D3D12RHIPipelineStateObject* d3d12pso = reinterpret_cast<D3D12RHIPipelineStateObject*>(pso.Get());

			m_NativeCommandList->SetPipelineState(d3d12pso->GetHandle());
		}

		void D3D12RHICommandList::SetMesh(RHIMesh::Ptr mesh)
		{
			assert(mesh != nullptr);

			D3D12RHIBuffer* vertexBuffer = reinterpret_cast<D3D12RHIBuffer*>(mesh->GetVertexBuffer().Get());
			D3D12RHIBuffer* indexBuffer = reinterpret_cast<D3D12RHIBuffer*>(mesh->GetIndexBuffer().Get());

			assert(vertexBuffer);
			assert(indexBuffer);

			D3D12_VERTEX_BUFFER_VIEW vbView = 
			{
				.BufferLocation = vertexBuffer->GetHandle()->GetGPUVirtualAddress(),
				.SizeInBytes = vertexBuffer->GetDesc().ElementSize * vertexBuffer->GetDesc().Count,
				.StrideInBytes = vertexBuffer->GetDesc().ElementSize
			};
			m_NativeCommandList->IASetVertexBuffers(0, 1, &vbView);

			D3D12_INDEX_BUFFER_VIEW ibView = 
			{
				.BufferLocation = indexBuffer->GetHandle()->GetGPUVirtualAddress(),
				.SizeInBytes = indexBuffer->GetDesc().ElementSize * indexBuffer->GetDesc().Count,
				.Format = ConvertToDXGIFormat(mesh->GetIndexFormat())
			};
			m_NativeCommandList->IASetIndexBuffer(&ibView);
		}

		void D3D12RHICommandList::SetGraphicsConstantBuffer(uint32_t ParameterIdx, RHIBuffer::Ptr buffer, uint32_t bufferOffset)
		{
			assert(buffer != nullptr);

			D3D12RHIBuffer* d3d12Buffer = reinterpret_cast<D3D12RHIBuffer*>(buffer.Get());
			D3D12_GPU_VIRTUAL_ADDRESS constantBufferAddress = d3d12Buffer->GetHandle()->GetGPUVirtualAddress() + bufferOffset * d3d12Buffer->GetDesc().ElementSize;
			m_NativeCommandList->SetGraphicsRootConstantBufferView(ParameterIdx, constantBufferAddress);
		}

		void D3D12RHICommandList::SetTechnique(RHITechnique::Ptr technique)
		{
			assert(technique != nullptr);

			D3D12RHITechnique* d3d12Technique = static_cast<D3D12RHITechnique*>(technique.Get());

			m_NativeCommandList->SetGraphicsRootSignature(d3d12Technique->GetRootSignature());
		}

		void D3D12RHICommandList::SetPrimitiveTopology(PrimitiveTopology topology)
		{
			m_NativeCommandList->IASetPrimitiveTopology(ConvertToD3D12PrimitiveTopology(topology));
		}

		void D3D12RHICommandList::DrawIndexedInstanced(
			uint32_t IndexCountPerInstance,
			uint32_t InstanceCount,
			uint32_t StartIndexLocation,
			int32_t BaseVertexLocation,
			uint32_t StartInstanceLocation
		)
		{
			m_NativeCommandList->DrawIndexedInstanced(
				IndexCountPerInstance,
				InstanceCount,
				StartIndexLocation,
				BaseVertexLocation,
				StartInstanceLocation);
		}

		RenderNativeObject D3D12RHICommandList::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandList.Get());
		}

		RefCountPtr<ID3D12GraphicsCommandList> D3D12RHICommandList::GetHandle() const
		{
			return m_NativeCommandList;
		}

		void D3D12RHICommandList::ValidateTextureState(D3D12RHITexture* texture, D3D12_RESOURCE_STATES state)
		{
			assert(texture != nullptr);

			if (texture->GetCurrentState() != state)
			{
				D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(texture->GetHandle(),
					texture->GetCurrentState(), state);
				m_NativeCommandList->ResourceBarrier(1, &ResBarrierTransition);

				texture->SetCurrentState(state);
			}
		}
	}
}