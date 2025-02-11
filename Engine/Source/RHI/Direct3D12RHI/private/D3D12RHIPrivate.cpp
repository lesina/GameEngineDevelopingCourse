#include <D3D12Mesh.h>
#include <D3D12Material.h>
#include <D3D12RHIPrivate.h>
#include <DirectXMath.h>
#include <Window/IWindow.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		void D3D12RHIPrivate::EnableDebugLayer() const
		{
#if defined(DEBUG) || defined(_DEBUG) 
			Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			assert(SUCCEEDED(hr));
			debugController->EnableDebugLayer();
#endif
		}

		void D3D12RHIPrivate::Init()
		{
			CreateFactory();
			CreateDevice();
			CreateFence();

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
			msQualityLevels.Format = m_BackBufferFormat;
			msQualityLevels.SampleCount = 4;
			msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			msQualityLevels.NumQualityLevels = 0;
			
			HRESULT hr = m_Device->CheckFeatureSupport(
				D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&msQualityLevels,
				sizeof(msQualityLevels));
			assert(SUCCEEDED(hr));

			m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
			assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

			CreateCommandObjects();
			CreateSwapChain();
			CreateRtvAndDsvDescriptorHeaps();
			BuildDescriptorHeaps();
			BuildConstantBuffers();

			hr = m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr);
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::CreateFactory()
		{
			HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_Factory));
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::CreateDevice()
		{
			HRESULT hr = D3D12CreateDevice(
				nullptr,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_Device));

			if (FAILED(hr))
			{
				Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter;
				hr = m_Factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
				assert(SUCCEEDED(hr));

				hr = D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_Device));

				assert(SUCCEEDED(hr));
			}
		}

		void D3D12RHIPrivate::CreateFence()
		{
			HRESULT hr = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(&m_Fence));
		}

		void D3D12RHIPrivate::CreateCommandObjects()
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			HRESULT hr = m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));
			assert(SUCCEEDED(hr));

			hr = m_Device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_DirectCmdListAlloc.GetAddressOf()));
			assert(SUCCEEDED(hr));

			hr = m_Device->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_DirectCmdListAlloc.Get(),
				nullptr,
				IID_PPV_ARGS(m_CommandList.GetAddressOf()));
			assert(SUCCEEDED(hr));

			m_CommandList->Close();
		}

		void D3D12RHIPrivate::CreateSwapChain()
		{
			m_SwapChain.Reset();

			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width = Core::MainWindowsApplication->GetWidth();
			sd.BufferDesc.Height = Core::MainWindowsApplication->GetHeight();
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferDesc.Format = m_BackBufferFormat;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
			sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = SwapChainBufferCount;
			sd.OutputWindow = reinterpret_cast<HWND>(Core::MainWindowsApplication->GetWindowHandle());
			sd.Windowed = true;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			HRESULT hr = m_Factory->CreateSwapChain(
				m_CommandQueue.Get(),
				&sd,
				m_SwapChain.GetAddressOf());
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::CreateRtvAndDsvDescriptorHeaps()
		{
			m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			m_DsvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_CbvSrvUavDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
			rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvHeapDesc.NodeMask = 0;
			HRESULT hr = m_Device->CreateDescriptorHeap(
				&rtvHeapDesc, IID_PPV_ARGS(m_RtvHeap.GetAddressOf()));
			assert(SUCCEEDED(hr));

			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvHeapDesc.NodeMask = 0;
			hr = m_Device->CreateDescriptorHeap(
				&dsvHeapDesc, IID_PPV_ARGS(m_DsvHeap.GetAddressOf()));
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::BuildDescriptorHeaps()
		{
			D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
			cbvHeapDesc.NumDescriptors = 1;
			cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			cbvHeapDesc.NodeMask = 0;
			HRESULT hr = m_Device->CreateDescriptorHeap(&cbvHeapDesc,
				IID_PPV_ARGS(&m_CbvHeap));
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::BuildConstantBuffers()
		{
			m_ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), 1, true);

			UINT objCBByteSize = D3D12Util::CalcConstantBufferByteSize(sizeof(ObjectConstants));

			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_ObjectCB->Resource()->GetGPUVirtualAddress();
			// Offset to the ith object constant buffer in the buffer.
			int boxCBufIndex = 0;
			cbAddress += boxCBufIndex * objCBByteSize;

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = D3D12Util::CalcConstantBufferByteSize(sizeof(ObjectConstants));

			m_Device->CreateConstantBufferView(
				&cbvDesc,
				m_CbvHeap->GetCPUDescriptorHandleForHeapStart());
		}

		Mesh::Ptr D3D12RHIPrivate::CreateMesh(
			void* vertices,
			uint16_t verticesCount,
			uint16_t vertexTypeSize,
			void* indices,
			uint16_t indicesCount,
			uint16_t indexTypeSize)
		{
			return std::make_shared<D3D12Mesh>(
				m_Device.Get(),
				m_CommandList.Get(),
				vertices,
				verticesCount,
				vertexTypeSize,
				indices,
				indicesCount,
				indexTypeSize);
		}

		Material::Ptr D3D12RHIPrivate::GetMaterial(const std::string& name)
		{
			return std::make_shared<D3D12Material>(name, m_Device.Get(), m_BackBufferFormat, m_DepthStencilFormat, m_4xMsaaState, m_4xMsaaQuality);
		}

		void D3D12RHIPrivate::ExecuteCommandLists()
		{
			HRESULT hr = m_CommandList->Close();
			assert(SUCCEEDED(hr));

			ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
			m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
		}

		void D3D12RHIPrivate::FlushCommandQueue()
		{
			m_CurrentFence++;

			HRESULT hr = m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence);
			assert(SUCCEEDED(hr));

			if (m_Fence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

				hr = m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle);
				assert(SUCCEEDED(hr));

				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
		}

		void D3D12RHIPrivate::Update(Mesh::Ptr mesh, Material::Ptr material)
		{
			D3D12Mesh d3d12Mesh = *reinterpret_cast<D3D12Mesh*>(mesh.get());
			D3D12Material d3d12Material = *reinterpret_cast<D3D12Material*>(material.get());

			float mTheta = 1.5f * DirectX::XM_PI;
			float mPhi = DirectX::XM_PIDIV4;
			float mRadius = 5.0f;

			// Convert Spherical to Cartesian coordinates.
			float x = mRadius * sinf(mPhi) * cosf(mTheta);
			float z = mRadius * sinf(mPhi) * sinf(mTheta);
			float y = mRadius * cosf(mPhi);

			// Build the view matrix.
			Math::Vector3f pos = Math::Vector3f(x, y, z);
			Math::Vector3f target = Math::Vector3f::Zero();
			Math::Vector3f up = Math::Vector3f(0.0f, 1.0f, 0.0f);

			// Projection and view matrices should be a part of Camera class
			Math::Matrix4x4f view = Core::Math::ViewMatrixLH(pos, target, up);
			Math::Matrix4x4f proj = Core::Math::ProjectionMatrixLH(0.25f * DirectX::XM_PI, Core::MainWindowsApplication->GetAspectRatio(), 1.0f, 1000.0f);

			Math::Matrix4x4f world = Math::Matrix4x4f::Identity();

            static float z_pos = 0;
            static float direction = 1;
            static float step = 1e-4;
            z_pos += direction * step;
            if (z_pos > 1.0f || z_pos < -1.0f) direction *= -1;
            world.SetElement(z_pos, 3, 0);

            static float angle = 0;
            float angle_step = 1e-4;
            angle += angle_step;
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);
            Math::Matrix4x4f rotation_matrix = Math::Matrix4x4f::Identity();
            rotation_matrix.SetElement(cosAngle, 0, 0);
            rotation_matrix.SetElement(sinAngle, 0, 2);
            rotation_matrix.SetElement(-sinAngle, 2, 0);
            rotation_matrix.SetElement(cosAngle, 2, 2);
            world = rotation_matrix * world;

			Math::Matrix4x4f worldViewProj = world * view * proj;

			ObjectConstants objConstants;
			objConstants.WorldViewProj = worldViewProj.Transpose();
			m_ObjectCB->CopyData(0, objConstants);

			HRESULT hr = m_DirectCmdListAlloc->Reset();
			assert(SUCCEEDED(hr));

			hr = m_CommandList->Reset(m_DirectCmdListAlloc.Get(), d3d12Material.GetPSO().Get());
			assert(SUCCEEDED(hr));

			m_CommandList->RSSetViewports(1, &m_ScreenViewport);
			m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

			D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_CommandList->ResourceBarrier(1, &ResBarrierTransition);

			m_CommandList->ClearRenderTargetView(CurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
			m_CommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

			D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = CurrentBackBufferView();
			D3D12_CPU_DESCRIPTOR_HANDLE currentDepthStencilView = DepthStencilView();
			m_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &currentDepthStencilView);

			ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

			m_CommandList->SetGraphicsRootSignature(d3d12Material.GetRootSignature().Get());

			D3D12_VERTEX_BUFFER_VIEW vbView = d3d12Mesh.VertexBufferView();
			D3D12_INDEX_BUFFER_VIEW ibView = d3d12Mesh.IndexBufferView();
			m_CommandList->IASetVertexBuffers(0, 1, &vbView);
			m_CommandList->IASetIndexBuffer(&ibView);
			m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_CommandList->SetGraphicsRootDescriptorTable(0, m_CbvHeap->GetGPUDescriptorHandleForHeapStart());

			m_CommandList->DrawIndexedInstanced(
				d3d12Mesh.GetIndexCount(),
				1, 0, 0, 0);

			ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			m_CommandList->ResourceBarrier(1, &ResBarrierTransition);

			ExecuteCommandLists();

			hr = m_SwapChain->Present(0, 0);
			assert(SUCCEEDED(hr));
			m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;
		}

		void D3D12RHIPrivate::OnResize()
		{
			assert(m_Device);
			assert(m_SwapChain);
			assert(m_DirectCmdListAlloc);

			FlushCommandQueue();

			HRESULT hr = m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr);
			assert(SUCCEEDED(hr));

			for (int i = 0; i < SwapChainBufferCount; ++i)
				m_SwapChainBuffer[i].Reset();
			m_DepthStencilBuffer.Reset();

			// Resize the swap chain.
			hr = m_SwapChain->ResizeBuffers(
				SwapChainBufferCount,
				Core::MainWindowsApplication->GetWidth(), Core::MainWindowsApplication->GetHeight(),
				m_BackBufferFormat,
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
			assert(SUCCEEDED(hr));

			m_CurrBackBuffer = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < SwapChainBufferCount; i++)
			{
				hr = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]));
				m_Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
			}

			D3D12_RESOURCE_DESC depthStencilDesc;
			depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthStencilDesc.Alignment = 0;
			depthStencilDesc.Width = Core::MainWindowsApplication->GetWidth();
			depthStencilDesc.Height = Core::MainWindowsApplication->GetHeight();
			depthStencilDesc.DepthOrArraySize = 1;
			depthStencilDesc.MipLevels = 1;

			depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
			depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
			depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE optClear;
			optClear.Format = m_DepthStencilFormat;
			optClear.DepthStencil.Depth = 1.0f;
			optClear.DepthStencil.Stencil = 0;

			D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			hr = m_Device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&depthStencilDesc,
				D3D12_RESOURCE_STATE_COMMON,
				&optClear,
				IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf()));
			assert(SUCCEEDED(hr));

			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format = m_DepthStencilFormat;
			dsvDesc.Texture2D.MipSlice = 0;
			m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

			D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			m_CommandList->ResourceBarrier(1, &ResBarrierTransition);

			ExecuteCommandLists();

			FlushCommandQueue();

			m_ScreenViewport.TopLeftX = 0;
			m_ScreenViewport.TopLeftY = 0;
			m_ScreenViewport.Width = static_cast<float>(Core::MainWindowsApplication->GetWidth());
			m_ScreenViewport.Height = static_cast<float>(Core::MainWindowsApplication->GetHeight());
			m_ScreenViewport.MinDepth = 0.0f;
			m_ScreenViewport.MaxDepth = 1.0f;

			m_ScissorRect = { 0, 0, Core::MainWindowsApplication->GetWidth(), Core::MainWindowsApplication->GetHeight() };
		}

		ID3D12Resource* D3D12RHIPrivate::CurrentBackBuffer()const
		{
			return m_SwapChainBuffer[m_CurrBackBuffer].Get();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12RHIPrivate::CurrentBackBufferView()const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(
				m_RtvHeap->GetCPUDescriptorHandleForHeapStart(),
				m_CurrBackBuffer,
				m_RtvDescriptorSize);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12RHIPrivate::DepthStencilView()const
		{
			return m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
	}
}