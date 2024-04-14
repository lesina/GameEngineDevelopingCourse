#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <d3dx12.h>
#include <DDSTextureLoader.h>

#include <Math/Vector.h>
#include <Window/IWindow.h>
#include <Mesh.h>
#include <Material.h>
#include <D3D12UploadBuffer.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIPrivate
		{
		public:
			D3D12RHIPrivate() = default;

		public:
			void EnableDebugLayer() const;
			void Init();

			Mesh::Ptr CreateMesh(
				void* vertices,
				uint16_t verticesCount,
				uint16_t vertexTypeSize,
				void* indices,
				uint16_t indicesCount,
				uint16_t indexTypeSize);
			
			Material::Ptr GetMaterial(const std::string& name);

			void Update(Mesh::Ptr mesh, Material::Ptr material);
			void ExecuteCommandLists();
			void FlushCommandQueue();
			void OnResize();

		private:
			Microsoft::WRL::ComPtr<IDXGIFactory4> m_Factory;
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
			
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CbvHeap = nullptr;
			std::unique_ptr<UploadBuffer<ObjectConstants>> m_ObjectCB = nullptr;
			
			UINT64 m_CurrentFence = 0;
			UINT m_RtvDescriptorSize = 0;
			UINT m_DsvDescriptorSize = 0;
			UINT m_CbvSrvUavDescriptorSize = 0;

			static const int SwapChainBufferCount = 2;
			int m_CurrBackBuffer = 0;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];
			Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

			bool m_4xMsaaState = false;
			UINT m_4xMsaaQuality = 0;

			DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			D3D12_VIEWPORT m_ScreenViewport;
			D3D12_RECT m_ScissorRect;

		private:
			void CreateFactory();
			void CreateDevice();
			void CreateFence();

			void CreateCommandObjects();
			void CreateSwapChain();
			void CreateRtvAndDsvDescriptorHeaps();

			void BuildDescriptorHeaps();
			void BuildConstantBuffers();

			ID3D12Resource* CurrentBackBuffer() const;
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
		};
	}
}