#include <D3D12Mesh.h>
#include <D3D12Material.h>
#include <D3D12RHIPrivate.h>
#include <DirectXMath.h>
#include <Camera.h>
#include <FileSystem.h>
#include <Matrix.h>
#include <RenderCore.h>
#include <Vector.h>
#include <Window/IWindow.h>

namespace GameEngine
{
	namespace Render::HAL
	{
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
			BuildConstantBuffers();

			// We will get rid of this later
			BuildRootSignature();
			BuildShadersAndInputLayout();
			BuildPSO();

			hr = m_CommandList->Reset(m_DirectCmdListAlloc[m_CurrBackBuffer].Get(), nullptr);
			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::BuildRootSignature()
		{
			CD3DX12_ROOT_PARAMETER slotRootParameter[2];

			slotRootParameter[0].InitAsConstantBufferView(0); // b0
			slotRootParameter[1].InitAsConstantBufferView(1); // b1

			CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
				serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

			if (errorBlob != nullptr)
			{
				::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
			assert(SUCCEEDED(hr));

			hr = m_Device->CreateRootSignature(
				0,
				serializedRootSig->GetBufferPointer(),
				serializedRootSig->GetBufferSize(),
				IID_PPV_ARGS(&m_RootSignature));

			assert(SUCCEEDED(hr));
		}

		void D3D12RHIPrivate::BuildShadersAndInputLayout()
		{
			HRESULT hr = S_OK;

			std::wstring shaderPath = Core::g_FileSystem->GetShaderPath("Object.hlsl");

			m_vsByteCode = D3D12Util::CompileShader(shaderPath, nullptr, "VS", "vs_5_0");
			m_psByteCode = D3D12Util::CompileShader(shaderPath, nullptr, "PS", "ps_5_0");

			m_InputLayout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};
		}

		void D3D12RHIPrivate::BuildPSO()
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
			ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
			psoDesc.InputLayout = { m_InputLayout.data(), (uint32_t)m_InputLayout.size() };
			psoDesc.pRootSignature = m_RootSignature.Get();
			psoDesc.VS =
			{
				reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()),
				m_vsByteCode->GetBufferSize()
			};
			psoDesc.PS =
			{
				reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()),
				m_psByteCode->GetBufferSize()
			};
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = m_BackBufferFormat;
			psoDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
			psoDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
			psoDesc.DSVFormat = m_DepthStencilFormat;
			HRESULT hr = m_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
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

			for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
			{
				hr = m_Device->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(m_DirectCmdListAlloc[i].GetAddressOf()));
				assert(SUCCEEDED(hr));
			}

			hr = m_Device->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_DirectCmdListAlloc[m_CurrBackBuffer].Get(),
				nullptr,
				IID_PPV_ARGS(m_CommandList.GetAddressOf()));
			assert(SUCCEEDED(hr));

			m_CommandList->Close();
		}

		void D3D12RHIPrivate::CreateSwapChain()
		{
			m_SwapChain.Reset();

			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width = Core::g_MainWindowsApplication->GetWidth();
			sd.BufferDesc.Height = Core::g_MainWindowsApplication->GetHeight();
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferDesc.Format = m_BackBufferFormat;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
			sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = RenderCore::g_FrameBufferCount;
			sd.OutputWindow = reinterpret_cast<HWND>(Core::g_MainWindowsApplication->GetWindowHandle());
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
			rtvHeapDesc.NumDescriptors = RenderCore::g_FrameBufferCount;
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

		void D3D12RHIPrivate::BuildConstantBuffers()
		{
			for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
			{
				m_ObjectCB[i] = std::make_unique<UploadBuffer<ObjectConstants>>(m_Device.Get(), RenderCore::g_MaximumRenderObjectCount, true);
				m_MaterialCB[i] = std::make_unique<UploadBuffer<MaterialConstants>>(m_Device.Get(), RenderCore::g_MaximumRenderObjectCount, true);
			}
		}

		D3D12Mesh* D3D12RHIPrivate::CreateMesh(
			RHIMesh::ID id,
			void* vertices,
			size_t verticesCount,
			uint32_t vertexTypeSize,
			void* indices,
			size_t indicesCount,
			uint32_t indexTypeSize)
		{
			HRESULT hr = m_CommandList->Reset(m_DirectCmdListAlloc[m_CurrBackBuffer].Get(), nullptr);
			assert(SUCCEEDED(hr));

			D3D12Mesh* mesh = new D3D12Mesh(
									m_Device.Get(),
									m_CommandList.Get(),
									id,
									vertices,
									verticesCount,
									vertexTypeSize,
									indices,
									indicesCount,
									indexTypeSize);

			ExecuteCommandLists();

			return mesh;
		}

		D3D12Material* D3D12RHIPrivate::CreateMaterial(RHIMaterial::ID id)
		{
			HRESULT hr = m_CommandList->Reset(m_DirectCmdListAlloc[m_CurrBackBuffer].Get(), nullptr);
			assert(SUCCEEDED(hr));

			D3D12Material* material = new D3D12Material(id);

			ExecuteCommandLists();

			return material;
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

		void D3D12RHIPrivate::BeginFrame()
		{
			HRESULT hr = m_DirectCmdListAlloc[m_CurrBackBuffer]->Reset();
			assert(SUCCEEDED(hr));

			hr = m_CommandList->Reset(m_DirectCmdListAlloc[m_CurrBackBuffer].Get(), m_PSO.Get());
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

			m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		}

		void D3D12RHIPrivate::EndFrame()
		{
			D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			m_CommandList->ResourceBarrier(1, &ResBarrierTransition);

			ExecuteCommandLists();

			HRESULT hr = m_SwapChain->Present(0, 0);
			assert(SUCCEEDED(hr));
			m_CurrBackBuffer = (m_CurrBackBuffer + 1) % RenderCore::g_FrameBufferCount;
		}

		void D3D12RHIPrivate::Draw(
			RHIMesh::ID meshID,
			const D3D12Mesh* mesh,
			RHIMaterial::ID materialID,
			const D3D12Material* material,
			const Math::Vector3f& position
		)
		{
			// Projection and view matrices should be a part of Camera class
			Math::Matrix4x4f view = Core::g_MainCamera->GetViewMatrix();
			Math::Matrix4x4f proj = Math::ProjectionMatrixLH(0.25f * DirectX::XM_PI, Core::g_MainWindowsApplication->GetAspectRatio(), 1.0f, 1000.0f);

			Math::Matrix4x4f world = Math::Matrix4x4f::Identity();
			world.SetElement(position.x, 3, 0);
			world.SetElement(position.y, 3, 1);
			world.SetElement(position.z, 3, 2);
			Math::Matrix4x4f worldViewProj = world * view * proj;

			ObjectConstants objConstants;
			objConstants.WorldViewProj = worldViewProj.Transpose();
			m_ObjectCB[m_CurrBackBuffer]->CopyData(meshID, objConstants);

			MaterialConstants matConstants;
			matConstants.Color = material->GetAlbedo();
			m_MaterialCB[m_CurrBackBuffer]->CopyData(materialID, matConstants);

			D3D12_VERTEX_BUFFER_VIEW vbView = mesh->VertexBufferView();
			D3D12_INDEX_BUFFER_VIEW ibView = mesh->IndexBufferView();
			m_CommandList->IASetVertexBuffers(0, 1, &vbView);
			m_CommandList->IASetIndexBuffer(&ibView);
			m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_ObjectCB[m_CurrBackBuffer]->Resource()->GetGPUVirtualAddress() + meshID * m_ObjectCB[m_CurrBackBuffer]->GetElementByteSize();
			D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = m_MaterialCB[m_CurrBackBuffer]->Resource()->GetGPUVirtualAddress() + materialID * m_MaterialCB[m_CurrBackBuffer]->GetElementByteSize();
			m_CommandList->SetGraphicsRootConstantBufferView(0, objCBAddress);
			m_CommandList->SetGraphicsRootConstantBufferView(1, matCBAddress);

			m_CommandList->DrawIndexedInstanced(
				mesh->GetIndexCount(),
				1, 0, 0, 0);
		}

		void D3D12RHIPrivate::OnResize()
		{
			assert(m_Device);
			assert(m_SwapChain);
			assert(m_DirectCmdListAlloc[m_CurrBackBuffer]);

			FlushCommandQueue();

			HRESULT hr = m_CommandList->Reset(m_DirectCmdListAlloc[m_CurrBackBuffer].Get(), nullptr);
			assert(SUCCEEDED(hr));

			for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
			{
				m_SwapChainBuffer[i].Reset();
			}
			m_DepthStencilBuffer.Reset();

			// Resize the swap chain.
			hr = m_SwapChain->ResizeBuffers(
				RenderCore::g_FrameBufferCount,
				Core::g_MainWindowsApplication->GetWidth(), Core::g_MainWindowsApplication->GetHeight(),
				m_BackBufferFormat,
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
			assert(SUCCEEDED(hr));

			m_CurrBackBuffer = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < RenderCore::g_FrameBufferCount; i++)
			{
				hr = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]));
				m_Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
			}

			D3D12_RESOURCE_DESC depthStencilDesc;
			depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthStencilDesc.Alignment = 0;
			depthStencilDesc.Width = Core::g_MainWindowsApplication->GetWidth();
			depthStencilDesc.Height = Core::g_MainWindowsApplication->GetHeight();
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
			m_ScreenViewport.Width = static_cast<float>(Core::g_MainWindowsApplication->GetWidth());
			m_ScreenViewport.Height = static_cast<float>(Core::g_MainWindowsApplication->GetHeight());
			m_ScreenViewport.MinDepth = 0.0f;
			m_ScreenViewport.MaxDepth = 1.0f;

			m_ScissorRect = { 0, 0, (LONG)Core::g_MainWindowsApplication->GetWidth(), (LONG)Core::g_MainWindowsApplication->GetHeight() };
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