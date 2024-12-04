#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <RHIBuffer.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIBuffer final : public RHIBuffer
		{
		public:
			using Ptr = RefCountPtr<D3D12RHIBuffer>;

		public:
			D3D12RHIBuffer() = delete;
			D3D12RHIBuffer(
				const Description& desc,
				RefCountPtr<ID3D12Resource> resourceDefault = nullptr,
				RefCountPtr<ID3D12Resource> resourceUpload = nullptr
			);

		public:
			virtual void CopyData(int elementIndex, void* data, uint32_t DataSize) override;
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Resource> GetHandle() const;

		private:
			RefCountPtr<ID3D12Resource> m_NativeResourceDefault = nullptr;
			RefCountPtr<ID3D12Resource> m_NativeResourceUpload = nullptr;
		};
	}
}