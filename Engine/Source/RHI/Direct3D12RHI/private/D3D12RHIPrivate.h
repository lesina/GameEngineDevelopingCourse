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

#include <Vector.h>
#include <Window/IWindow.h>
#include <RenderCore.h>
#include <RHIMesh.h>
#include <RHIMaterial.h>
#include <D3D12UploadBuffer.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12Mesh;
		class D3D12Material;

		class D3D12RHIPrivate
		{
		public:
			D3D12RHIPrivate() = default;

		public:
			void EnableDebugLayer() const;
			void Init();

			D3D12Mesh* CreateMesh(
				RHIMesh::ID id,
				void* vertices,
				size_t verticesCount,
				uint32_t vertexTypeSize,
				void* indices,
				size_t indicesCount,
				uint32_t indexTypeSize);
			
			D3D12Material* CreateMaterial(RHIMaterial::ID id);

			void Draw(
				RHIMesh::ID meshID,
				const D3D12Mesh* mesh,
				RHIMaterial::ID materialID,
				const D3D12Material* material,
				const Math::Vector3f& position
			);
			void ExecuteCommandLists();
			void FlushCommandQueue();
			void OnResize();
			void BeginFrame();
			void EndFrame();

		private:
			Microsoft::WRL::ComPtr<IDXGIFactory4> m_Factory;
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc[RenderCore::g_FrameBufferCount];
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
			
			std::unique_ptr<UploadBuffer<ObjectConstants>> m_ObjectCB[RenderCore::g_FrameBufferCount];
			std::unique_ptr<UploadBuffer<MaterialConstants>> m_MaterialCB[RenderCore::g_FrameBufferCount];
			
			uint64_t m_CurrentFence = 0;
			uint32_t m_RtvDescriptorSize = 0;
			uint32_t m_DsvDescriptorSize = 0;
			uint32_t m_CbvSrvUavDescriptorSize = 0;

			int m_CurrBackBuffer = 0;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[RenderCore::g_FrameBufferCount];
			Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

			bool m_4xMsaaState = false;
			uint32_t m_4xMsaaQuality = 0;

			DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			D3D12_VIEWPORT m_ScreenViewport;
			D3D12_RECT m_ScissorRect;

			// Shader Info
			Microsoft::WRL::ComPtr<ID3DBlob> m_vsByteCode = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> m_psByteCode = nullptr;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO = nullptr;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;

			std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

			// These functions are temporal
			// They basically hardcode only one renderpass
			void BuildRootSignature();
			void BuildShadersAndInputLayout();
			void BuildPSO();

			// End shader Info

		private:
			void CreateFactory();
			void CreateDevice();
			void CreateFence();

			void CreateCommandObjects();
			void CreateSwapChain();
			void CreateRtvAndDsvDescriptorHeaps();

			void BuildConstantBuffers();

			ID3D12Resource* CurrentBackBuffer() const;
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
		};
	}
}