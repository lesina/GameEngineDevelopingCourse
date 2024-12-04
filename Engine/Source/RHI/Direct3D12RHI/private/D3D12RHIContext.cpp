#include <array.h>
#include <Debug/Assertions.h>
#include <Geometry.h>
#include <Debug/Console.h>
#include <Debug/Console.h>
#include <FileSystem.h>
#include <Vector.h>
#include <RHICommon.h>
#include <D3D12DescriptorHeap.h>
#include <D3D12RHICore.h>
#include <D3D12RHIBuffer.h>
#include <D3D12RHIFactory.h>
#include <D3D12RHIDevice.h>
#include <D3D12RHISwapChain.h>
#include <D3D12RHIFence.h>
#include <D3D12RHICommandQueue.h>
#include <D3D12RHICommandAllocator.h>
#include <D3D12RHICommandList.h>
#include <D3D12RHITexture.h>
#include <D3D12RHIContext.h>
#include <D3D12RHITechnique.h>
#include <D3D12RHIPipelineStateObject.h>
#include <D3D12Util.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHIContext::D3D12RHIContext()
		{
			EnableDebugLayer();

			m_Factory = new D3D12RHIFactory();
			m_Device = new D3D12RHIDevice(m_Factory);

			m_Fence = new D3D12RHIFence(m_Device);

			m_CommandQueue = new D3D12RHICommandQueue(m_Device);
			m_CommandList = new D3D12RHICommandList(m_Device);
			m_CommandList->Reset();

			m_SwapChain = new D3D12RHISwapChain(m_Device, m_Factory, m_CommandQueue);
			m_SwapChain->Resize(m_Device, 1.f, 1.f);

			m_RtvHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumRenderTargets, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			m_DsvHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumDepthStencilTargets, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_SrvCbvUavHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumCBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			m_SamplerHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumSamplers, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}

		void D3D12RHIContext::EnableDebugLayer() const
		{
#if DEBUG
			RefCountPtr<ID3D12Debug> debugController;
			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			assert(SUCCEEDED(hr));
			debugController->EnableDebugLayer();
#endif
		}

		D3D12RHIContext::~D3D12RHIContext()
		{
		}

		RHIDevice::Ptr D3D12RHIContext::GetDevice() const
		{
			return m_Device;
		}

		RHIFactory::Ptr D3D12RHIContext::GetFactory() const
		{
			return m_Factory;
		}

		RHISwapChain::Ptr D3D12RHIContext::GetSwapChain() const
		{
			return m_SwapChain;
		}

		RHIFence::Ptr D3D12RHIContext::GetFence() const
		{
			return m_Fence;
		}

		RHICommandQueue::Ptr D3D12RHIContext::GetCommandQueue() const
		{
			return m_CommandQueue;
		}

		RHICommandList::Ptr D3D12RHIContext::GetCommandList() const
		{
			return m_CommandList;
		}

		RHIMesh::Ptr D3D12RHIContext::CreateMesh(
			const RHIMesh::VertexBufferDescription& vertexDesc,
			const RHIMesh::IndexBufferDescription& indexDesc
		)
		{
			assert(vertexDesc.initData);
			assert(indexDesc.initData);

			m_CommandList->Reset();

			RHIBuffer::Ptr vertexBuffer = CreateBuffer(
				{
					.Count = vertexDesc.Count,
					.ElementSize = vertexDesc.ElementSize,
					.UsageFlag = HAL::RHIBuffer::UsageFlag::GpuReadOnly,
					.initData = vertexDesc.initData
				}
			);

			RHIBuffer::Ptr indexBuffer = CreateBuffer(
				{
					.Count = indexDesc.Count,
					.ElementSize = GetFormatSize(indexDesc.Format),
					.UsageFlag = HAL::RHIBuffer::UsageFlag::GpuReadOnly,
					.initData = indexDesc.initData
				}
			);

			m_CommandList->Close();
			m_CommandQueue->ExecuteCommandLists({ m_CommandList });

			m_Fence->Sync(m_CommandQueue);

			return RHIMesh::Ptr(new RHIMesh(vertexBuffer, indexBuffer, indexDesc.Format));
		}

		RHITexture::Ptr D3D12RHIContext::CreateTexture(const RHITexture::Description& description)
		{
			RefCountPtr<ID3D12Resource> textureResource;

			D3D12_RESOURCE_DESC d3d12textureDesc;
			d3d12textureDesc.Dimension = description.Dimension == RHITexture::Dimensions::Two ? D3D12_RESOURCE_DIMENSION_TEXTURE2D : D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			d3d12textureDesc.Alignment = 0;
			d3d12textureDesc.Width = description.Width;
			d3d12textureDesc.Height = description.Height;
			d3d12textureDesc.DepthOrArraySize = 1;
			d3d12textureDesc.MipLevels = 1;
			d3d12textureDesc.Format = ConvertToDXGIFormat(description.Format);
			d3d12textureDesc.SampleDesc.Count = 1;
			d3d12textureDesc.SampleDesc.Quality = 0;
			d3d12textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			d3d12textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			if (description.Flags & RHITexture::UsageFlags::DepthStencil)
			{
				d3d12textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			}

			if (description.Flags & RHITexture::UsageFlags::RenderTarget)
			{
				d3d12textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			}

			D3D12_CLEAR_VALUE optClear;
			optClear.Format = ConvertToDXGIFormat(description.Format);
			optClear.DepthStencil.Depth = 1.0f;
			optClear.DepthStencil.Stencil = 0;

			D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			HRESULT hr = m_Device->GetHandle()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12textureDesc,
				D3D12_RESOURCE_STATE_COMMON,
				&optClear,
				IID_PPV_ARGS(textureResource.GetAddressOf()));
			assert(SUCCEEDED(hr));

			D3D12_CPU_DESCRIPTOR_HANDLE depthStencilCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			// Creating Depth Stencil
			if (description.Flags & RHITexture::UsageFlags::DepthStencil)
			{
				D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
				dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
				dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Format = ConvertToDXGIFormat(description.Format);
				dsvDesc.Texture2D.MipSlice = 0;

				depthStencilCpuDesciptor = m_DsvHeap->AllocateNewDescriptor();

				m_Device->GetHandle()->CreateDepthStencilView(textureResource.Get(), &dsvDesc, depthStencilCpuDesciptor);
			}

			D3D12_CPU_DESCRIPTOR_HANDLE srvCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			// Creating Shader Resource
			if (description.Flags & RHITexture::UsageFlags::ShaderResource)
			{
				assert(0 && "Shader Resource View: Not supported");
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;

				srvCpuDesciptor = m_SrvCbvUavHeap->AllocateNewDescriptor();
				m_Device->GetHandle()->CreateShaderResourceView(textureResource.Get(), &srvDesc, srvCpuDesciptor);
			}

			D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			// Creating Render Target
			if (description.Flags & RHITexture::UsageFlags::RenderTarget)
			{
				assert(0 && "Render Target View: Not supported");

				D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;

				rtvCpuDesciptor = m_RtvHeap->AllocateNewDescriptor();
				m_Device->GetHandle()->CreateRenderTargetView(textureResource.Get(), &rtvDesc, rtvCpuDesciptor);
			}

			return D3D12RHITexture::Ptr(new D3D12RHITexture(description, textureResource, srvCpuDesciptor, rtvCpuDesciptor, depthStencilCpuDesciptor));
		}

		RHIBuffer::Ptr D3D12RHIContext::CreateBuffer(RHIBuffer::Description&& description)
		{
			if (description.UsageFlag == RHIBuffer::UsageFlag::ConstantBuffer)
			{
				// Should be multiple of 256
				description.ElementSize = (description.ElementSize + 255) & ~255;
			}

			uint64_t elementSize = description.ElementSize;

			RefCountPtr<ID3D12Resource> uploadBuffer;

			D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(elementSize * description.Count);
			HRESULT hr = m_Device->GetHandle()->CreateCommittedResource(
				&HeapProp,
				D3D12_HEAP_FLAG_NONE,
				&ResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuffer));
			assert(SUCCEEDED(hr));

			if (description.UsageFlag == RHIBuffer::UsageFlag::GpuReadOnly)
			{
				assert(description.initData != nullptr);

				RefCountPtr<ID3D12Resource> defaultBuffer;

				D3D12_HEAP_PROPERTIES HeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				D3D12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(elementSize * description.Count);
				HRESULT hr = m_Device->GetHandle()->CreateCommittedResource(
					&HeapProp,
					D3D12_HEAP_FLAG_NONE,
					&ResDesc,
					D3D12_RESOURCE_STATE_COMMON,
					nullptr,
					IID_PPV_ARGS(&defaultBuffer));
				assert(SUCCEEDED(hr));

				D3D12_SUBRESOURCE_DATA subResourceData = {};
				subResourceData.pData = description.initData;
				subResourceData.RowPitch = elementSize * description.Count;
				subResourceData.SlicePitch = subResourceData.RowPitch;

				D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
				m_CommandList->GetHandle()->ResourceBarrier(1, &ResBarrierTransition);
				UpdateSubresources<1>(m_CommandList->GetHandle(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

				ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
					D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
				m_CommandList->GetHandle()->ResourceBarrier(1, &ResBarrierTransition);

				return D3D12RHIBuffer::Ptr(new D3D12RHIBuffer(description, defaultBuffer, uploadBuffer));
			}

			return D3D12RHIBuffer::Ptr(new D3D12RHIBuffer(description, nullptr, uploadBuffer));
		}

		RHITechnique::Ptr D3D12RHIContext::CreateTechnique(
			const RHITechnique::ShaderInfo& shaderInfo,
			const RHITechnique::InputLayout& inputLayout,
			const RHITechnique::RootSignature& rootSignature
		)
		{
			// Root Signature
			CD3DX12_ROOT_PARAMETER* slotRootParameter = new CD3DX12_ROOT_PARAMETER[rootSignature.size()];

			for (uint64_t rootSigIdx = 0; rootSigIdx < rootSignature.size(); ++rootSigIdx)
			{
				if (rootSignature[rootSigIdx].IsConstantBuffer)
				{
					slotRootParameter[rootSigIdx].InitAsConstantBufferView(rootSignature[rootSigIdx].SlotIndex, rootSignature[rootSigIdx].SpaceIndex);
				}
				else
				{
					ASSERT_NOT_IMPLEMENTED;
				}
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(rootSignature.size(), slotRootParameter, 0, nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			RefCountPtr<ID3DBlob> serializedRootSig = nullptr;
			RefCountPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
				serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

			if (errorBlob != nullptr)
			{
				Core::Console::PrintDebug((char*)errorBlob->GetBufferPointer());
			}
			assert(SUCCEEDED(hr));

			RefCountPtr<ID3D12RootSignature> d3d12RootSignature;
			hr = m_Device->GetHandle()->CreateRootSignature(
				0,
				serializedRootSig->GetBufferPointer(),
				serializedRootSig->GetBufferSize(),
				IID_PPV_ARGS(&d3d12RootSignature));

			assert(SUCCEEDED(hr));

			// Shaders
			D3D12RHITechnique::ShaderBlobList shaderBlobs;

			for (const RHITechnique::ShaderInfoDescription& shaderDesc : shaderInfo)
			{
				std::wstring shaderPath = Core::g_FileSystem->GetShaderPath(shaderDesc.ShaderFile);

				shaderBlobs.emplace(shaderDesc.Type, D3D12Util::CompileShader(shaderPath, nullptr, shaderDesc.EntryPoint, GetShaderTarget(shaderDesc.Type)));
			}

			// Input Layout
			D3D12RHITechnique::InputLayoutList inputLayoutList;
			uint32_t byteOffset = 0;
			for (const RHITechnique::InputLayoutDescription& inputLayoutDesc : inputLayout)
			{
				inputLayoutList.emplace_back(
						inputLayoutDesc.SemanticName.c_str(),
						inputLayoutDesc.Index,
						ConvertToDXGIFormat(inputLayoutDesc.Format),
						inputLayoutDesc.InputSlot,
						byteOffset,
						ConvertToD3D12InputClassification(inputLayoutDesc.InputSlotClass),
						inputLayoutDesc.InstanceDataStepRate
				);

				byteOffset += GetFormatSize(inputLayoutDesc.Format);
			}

			return D3D12RHITechnique::Ptr(new D3D12RHITechnique(shaderInfo, inputLayout, rootSignature, d3d12RootSignature, std::move(shaderBlobs), std::move(inputLayoutList)));
		}

		RHIPipelineStateObject::Ptr D3D12RHIContext::CreatePSO(const RHIPipelineStateObject::Description& description)
		{
			D3D12RHITechnique* d3d12Technique = static_cast<D3D12RHITechnique*>(description.Technique.Get());

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
			ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

			psoDesc.InputLayout = { d3d12Technique->GetInputLayout().data(), (uint32_t)d3d12Technique->GetInputLayout().size() };
			psoDesc.pRootSignature = d3d12Technique->GetRootSignature().Get();
			if (d3d12Technique->GetShaderBlobs().contains(RHITechnique::ShaderInfoDescription::ShaderType::VertexShader))
			{
				psoDesc.VS =
				{
					.pShaderBytecode = reinterpret_cast<uint8_t*>(d3d12Technique->GetShaderBlobs().at(RHITechnique::ShaderInfoDescription::ShaderType::VertexShader)->GetBufferPointer()),
					.BytecodeLength = d3d12Technique->GetShaderBlobs().at(RHITechnique::ShaderInfoDescription::ShaderType::VertexShader)->GetBufferSize()
				};
			}
			if (d3d12Technique->GetShaderBlobs().contains(RHITechnique::ShaderInfoDescription::ShaderType::PixelShader))
			{
				psoDesc.PS =
				{
					.pShaderBytecode = reinterpret_cast<uint8_t*>(d3d12Technique->GetShaderBlobs().at(RHITechnique::ShaderInfoDescription::ShaderType::PixelShader)->GetBufferPointer()),
					.BytecodeLength = d3d12Technique->GetShaderBlobs().at(RHITechnique::ShaderInfoDescription::ShaderType::PixelShader)->GetBufferSize()
				};
			}

			psoDesc.RasterizerState = ConvertToD3D12RasterState(description.RasterState);
			psoDesc.BlendState = ConvertToD3D12BlendState(description.BlendState);
			psoDesc.DepthStencilState = ConvertToD3D12DepthStencilDesc(description.DepthStencilState);
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = ConvertToD3D12PrimitiveTopologyType(description.PrimitiveTopology);

			psoDesc.NumRenderTargets = description.NumRenderTargets;
			for (size_t rtIdx = 0; rtIdx < description.NumRenderTargets; ++rtIdx)
			{
				psoDesc.RTVFormats[rtIdx] = ConvertToDXGIFormat(description.RTVFormats[rtIdx]);
			}

			psoDesc.SampleDesc.Count = 1;
			psoDesc.SampleDesc.Quality = 0;
			psoDesc.DSVFormat = ConvertToDXGIFormat(description.DSVFormat);

			RefCountPtr<ID3D12PipelineState> pso;
			HRESULT hr = m_Device->GetHandle()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));
			assert(SUCCEEDED(hr));

			return D3D12RHIPipelineStateObject::Ptr(new D3D12RHIPipelineStateObject(description, pso));
		}
	}
}
