#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <RHITexture.h>
#include <D3D12RHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12DescriptorHeap final : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<D3D12DescriptorHeap>;

		public:
			D3D12DescriptorHeap() = delete;
			D3D12DescriptorHeap(
				D3D12RHIDevice::Ptr device,
				uint32_t NumDescriptors,
				D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType
			);

		public:
			[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE AllocateNewDescriptor();

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12DescriptorHeap> GetHandle() const;

		private:
			uint32_t m_DescriptorSize = 0;
			uint32_t m_HeapIdx = 0;
			uint32_t m_MaximumDescriptors = 0;
			RefCountPtr<ID3D12DescriptorHeap> m_NativeHeap;
			D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorHeapType;
		};
	}
}