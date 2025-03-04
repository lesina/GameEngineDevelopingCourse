#include <array.h>
#include <Debug/Assertions.h>
#include <Geometry.h>
#include <Debug/Console.h>
#include <FileSystem.h>
#include <Vector.h>
#include <RHICommon.h>
#include <D3D12DescriptorHeap.h>
#include <D3D12Core.h>
#include <D3D12Buffer.h>
#include <D3D12Factory.h>
#include <D3D12Device.h>
#include <D3D12SwapChain.h>
#include <D3D12Fence.h>
#include <D3D12CommandQueue.h>
#include <D3D12CommandAllocator.h>
#include <D3D12CommandList.h>
#include <D3D12Texture.h>
#include <D3D12Context.h>
#include <D3D12Technique.h>
#include <D3D12PipelineStateObject.h>
#include <D3D12Util.h>

namespace cqe
{
	namespace Render::RHI
	{
		using namespace Core;

		D3D12Context::D3D12Context()
		{
			EnableDebugLayer();

			m_Factory = new D3D12Factory();
			m_Device = new D3D12Device(m_Factory);

			m_Fence = new D3D12Fence(m_Device);

			m_CommandQueue = new D3D12CommandQueue(m_Device);
			m_CommandList = new D3D12CommandList(m_Device);
			m_CommandList->Reset();

			m_SwapChain = new D3D12SwapChain(m_Device, m_Factory, m_CommandQueue);
			m_SwapChain->Resize(m_Device, 1.f, 1.f);

			m_RtvHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumRenderTargets, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			m_DsvHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumDepthStencilTargets, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_SrvCbvUavHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumCBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			m_SamplerHeap = new D3D12DescriptorHeap(m_Device, RenderCore::g_MaximumSamplers, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}

		void D3D12Context::EnableDebugLayer() const
		{
#if DEBUG
			RefCountPtr<ID3D12Debug> debugController;
			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			assert(SUCCEEDED(hr));
			debugController->EnableDebugLayer();
#endif
		}

		D3D12Context::~D3D12Context()
		{
		}

		bool D3D12Context::CheckMinimalRequirements() const
		{
			D3D_FEATURE_LEVEL levels[] =
			{
				D3D_FEATURE_LEVEL_12_2,
				D3D_FEATURE_LEVEL_12_1,
			};
			D3D12_FEATURE_DATA_FEATURE_LEVELS supportedLevels;
			supportedLevels.NumFeatureLevels = sizeof(levels) / sizeof(D3D_FEATURE_LEVEL);
			supportedLevels.pFeatureLevelsRequested = levels;
			m_Device->GetHandle()->CheckFeatureSupport(
				D3D12_FEATURE_FEATURE_LEVELS,
				&supportedLevels,
				sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS)
			);

			D3D12_FEATURE_DATA_SHADER_MODEL shaderModelFeature{ .HighestShaderModel = D3D_SHADER_MODEL_6_6 };
			HRESULT hr = m_Device->GetHandle()->CheckFeatureSupport(
				D3D12_FEATURE_SHADER_MODEL,
				&shaderModelFeature,
				sizeof(D3D12_FEATURE_DATA_SHADER_MODEL)
			);
			assert(SUCCEEDED(hr));

			return shaderModelFeature.HighestShaderModel >= D3D_SHADER_MODEL_6_6
				&& supportedLevels.MaxSupportedFeatureLevel >= D3D_FEATURE_LEVEL_12_1;
		}

		Device::Ptr D3D12Context::GetDevice() const
		{
			return m_Device;
		}

		Factory::Ptr D3D12Context::GetFactory() const
		{
			return m_Factory;
		}

		SwapChain::Ptr D3D12Context::GetSwapChain() const
		{
			return m_SwapChain;
		}

		Fence::Ptr D3D12Context::GetFence() const
		{
			return m_Fence;
		}

		CommandQueue::Ptr D3D12Context::GetCommandQueue() const
		{
			return m_CommandQueue;
		}

		CommandList::Ptr D3D12Context::GetCommandList() const
		{
			return m_CommandList;
		}

		Mesh::Ptr D3D12Context::CreateMesh(
			const Mesh::VertexBufferDescription& vertexDesc,
			const Mesh::IndexBufferDescription& indexDesc
		)
		{
			assert(vertexDesc.initData);
			assert(indexDesc.initData);

			m_CommandList->Reset();

			Buffer::Ptr vertexBuffer = CreateBuffer(
				{
					.Count = vertexDesc.Count,
					.ElementSize = vertexDesc.ElementSize,
					.UsageFlag = RHI::Buffer::UsageFlag::GpuReadOnly,
					.initData = vertexDesc.initData
				}
			);

			Buffer::Ptr indexBuffer = CreateBuffer(
				{
					.Count = indexDesc.Count,
					.ElementSize = GetFormatSize(indexDesc.Format),
					.UsageFlag = RHI::Buffer::UsageFlag::GpuReadOnly,
					.initData = indexDesc.initData
				}
			);

			m_CommandList->Close();
			m_CommandQueue->ExecuteCommandLists({ m_CommandList });

			m_Fence->Sync(m_CommandQueue);

			return Mesh::Ptr(new Mesh(vertexBuffer, indexBuffer, indexDesc.Format));
		}

		Texture* D3D12Context::CreateTexture(const Texture::Description& description)
		{
			RefCountPtr<ID3D12Resource> textureResource;

			D3D12_RESOURCE_DESC d3d12textureDesc;
			d3d12textureDesc.Dimension = description.Dimension == Texture::Dimensions::Two ? D3D12_RESOURCE_DIMENSION_TEXTURE2D : D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			d3d12textureDesc.Alignment = 0;
			d3d12textureDesc.Width = description.Width;
			d3d12textureDesc.Height = description.Height;
			d3d12textureDesc.DepthOrArraySize = 1;
			d3d12textureDesc.MipLevels = description.MipLevels;
			d3d12textureDesc.Format = ConvertToDXGIFormat(description.Format);
			d3d12textureDesc.SampleDesc.Count = 1;
			d3d12textureDesc.SampleDesc.Quality = 0;
			d3d12textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			d3d12textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			if (description.Flags & Texture::UsageFlags::DepthStencil)
			{
				d3d12textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			}

			if (description.Flags & Texture::UsageFlags::RenderTarget)
			{
				d3d12textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			}

			D3D12_CLEAR_VALUE optClear;
			optClear.Format = ConvertToDXGIFormat(description.Format);
			optClear.DepthStencil.Depth = 1.0f;
			optClear.DepthStencil.Stencil = 0;

			bool noClear = description.Flags & Texture::UsageFlags::ShaderResource;

			D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			HRESULT hr = m_Device->GetHandle()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&d3d12textureDesc,
				D3D12_RESOURCE_STATE_COMMON,
				noClear ? nullptr : &optClear,
				IID_PPV_ARGS(textureResource.GetAddressOf()));
			assert(SUCCEEDED(hr));

			D3D12_CPU_DESCRIPTOR_HANDLE depthStencilCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			D3D12_GPU_DESCRIPTOR_HANDLE depthStencilGpuDesciptor = D3D12_GPU_DESCRIPTOR_HANDLE(0);
			// Creating Depth Stencil
			if (description.Flags & Texture::UsageFlags::DepthStencil)
			{
				D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
				dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
				dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Format = ConvertToDXGIFormat(description.Format);
				dsvDesc.Texture2D.MipSlice = 0;

				m_DsvHeap->Alloc(&depthStencilCpuDesciptor, &depthStencilGpuDesciptor);

				m_Device->GetHandle()->CreateDepthStencilView(textureResource.Get(), &dsvDesc, depthStencilCpuDesciptor);
			}

			D3D12_CPU_DESCRIPTOR_HANDLE srvCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuDesciptor = D3D12_GPU_DESCRIPTOR_HANDLE(0);
			// Creating Shader Resource
			if (description.Flags & Texture::UsageFlags::ShaderResource)
			{
				assert((description.Dimension == Texture::Dimensions::Two || description.Dimension == Texture::Dimensions::Three) && "Currently only 2D and 3D SRVs are supported");

				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc
				{
					.Format = ConvertToDXGIFormat(description.Format),
					.ViewDimension = description.Dimension == Texture::Dimensions::Two ? D3D12_SRV_DIMENSION_TEXTURE2D : D3D12_SRV_DIMENSION_TEXTURE3D,
					.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
				};

				switch (description.Dimension)
				{
				case Texture::Dimensions::Two:
					srvDesc.Texture2D.MipLevels = description.MipLevels;
					srvDesc.Texture2D.MostDetailedMip = 0;
					srvDesc.Texture2D.PlaneSlice = 0;
					srvDesc.Texture2D.ResourceMinLODClamp = 0;
					break;
				case Texture::Dimensions::Three:
					srvDesc.Texture3D.MipLevels = description.MipLevels;
					srvDesc.Texture3D.MostDetailedMip = 0;
					srvDesc.Texture3D.ResourceMinLODClamp = 0;
					break;
				}

				m_SrvCbvUavHeap->Alloc(&srvCpuDesciptor, &srvGpuDesciptor);
				m_Device->GetHandle()->CreateShaderResourceView(textureResource.Get(), &srvDesc, srvCpuDesciptor);
			}

			D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuDesciptor = D3D12_CPU_DESCRIPTOR_HANDLE(0);
			D3D12_GPU_DESCRIPTOR_HANDLE rtvGpuDesciptor = D3D12_GPU_DESCRIPTOR_HANDLE(0);
			// Creating Render Target
			if (description.Flags & Texture::UsageFlags::RenderTarget)
			{
				assert(0 && "Render Target View: Not supported");

				D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;

				m_RtvHeap->Alloc(&rtvCpuDesciptor, &rtvGpuDesciptor);
				m_Device->GetHandle()->CreateRenderTargetView(textureResource.Get(), &rtvDesc, rtvCpuDesciptor);
			}

			return new D3D12Texture(description, textureResource, rtvCpuDesciptor, srvCpuDesciptor, depthStencilCpuDesciptor, srvGpuDesciptor);
		}

		void D3D12Context::FreeTexture(const Texture* texture, Texture::UsageFlags::Flag usageFlag)
		{
			const D3D12Texture* d3d12Texture = static_cast<const D3D12Texture*>(texture);

			if (usageFlag & Texture::UsageFlags::DepthStencil)
			{
				m_DsvHeap->Free(d3d12Texture->GetDepthStencilView());
			}

			if (usageFlag & Texture::UsageFlags::ShaderResource)
			{
				m_SrvCbvUavHeap->Free(d3d12Texture->GetShaderResourceView(), d3d12Texture->GetGpuShaderResourceView());
			}

			if (usageFlag & Texture::UsageFlags::RenderTarget)
			{
				m_RtvHeap->Free(d3d12Texture->GetRenderTargetView());
			}
		}

		void D3D12Context::LoadTextureFromFile(Texture* texture, const std::wstring&& filePath)
		{
			D3D12Texture* d3d12Texture = static_cast<D3D12Texture*>(texture);
			assert(d3d12Texture != nullptr);
			
			std::vector<D3D12_SUBRESOURCE_DATA> subresourceData;
			std::unique_ptr<uint8_t[]> ddsData;
			HRESULT hr = DirectX::LoadDDSTextureFromFile(m_Device->GetHandle(), filePath.c_str(), d3d12Texture->GetHandle().ReleaseAndGetAddressOf(), ddsData, subresourceData);
			assert(SUCCEEDED(hr));

			const uint64_t dataSize = GetRequiredIntermediateSize(d3d12Texture->GetHandle().Get(), 0, subresourceData.size());

			m_CommandList->Reset();

			Buffer::Ptr uploadBuffer = CreateBuffer({
					.Count = 1,
					.ElementSize = static_cast<uint32_t>(dataSize),
					.UsageFlag = Buffer::UsageFlag::CpuWrite
				});

			ID3D12Resource* uploadRes = reinterpret_cast<ID3D12Resource*>(uploadBuffer.Get()->GetNativeObject().GetPtr());

			UpdateSubresources(m_CommandList->GetHandle(), d3d12Texture->GetHandle().Get(), uploadRes, 0, 0, static_cast<uint32_t>(subresourceData.size()), subresourceData.data());

			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(d3d12Texture->GetHandle().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			m_CommandList->GetHandle()->ResourceBarrier(1, &barrier);

			m_CommandList->Close();
			m_CommandQueue->ExecuteCommandLists({ m_CommandList });

			m_Fence->Sync(m_CommandQueue);

			d3d12Texture->SetCurrentState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}

		Buffer::Ptr D3D12Context::CreateBuffer(Buffer::Description&& description)
		{
			if (description.UsageFlag == Buffer::UsageFlag::ConstantBuffer)
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

			if (description.UsageFlag == Buffer::UsageFlag::GpuReadOnly)
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

				return D3D12Buffer::Ptr(new D3D12Buffer(description, defaultBuffer, uploadBuffer));
			}

			return D3D12Buffer::Ptr(new D3D12Buffer(description, nullptr, uploadBuffer));
		}

		Technique::Ptr D3D12Context::CreateTechnique(
			const Technique::ShaderInfo& shaderInfo,
			const Technique::InputLayout& inputLayout,
			const Technique::RootSignatureDescription& rootSignatureDescription
		)
		{
			// Shaders
			D3D12Technique::ShaderBlobList shaderBlobs;

			for (const Technique::ShaderInfoDescription& shaderDesc : shaderInfo)
			{
				std::wstring shaderPath = Core::g_FileSystem->GetShaderPath(shaderDesc.ShaderFile);

				RefCountPtr<ID3DBlob> shader = D3D12Util::CompileShader(shaderPath, nullptr, shaderDesc.EntryPoint, GetShaderTarget(shaderDesc.Type));
				shaderBlobs.emplace(shaderDesc.Type, shader);
			}

			// Root Signature
			RefCountPtr<ID3D12RootSignature> d3d12RootSignature;
			{
				RefCountPtr<ID3DBlob> rootSignatureBlob = D3D12Util::CompileRootSignature(rootSignatureDescription.Type, Core::g_FileSystem->GetShaderPath(rootSignatureDescription.Type + ".ihlsl"));

				HRESULT hr = m_Device->GetHandle()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&d3d12RootSignature));
				assert(SUCCEEDED(hr));
			}

			// Input Layout
			D3D12Technique::InputLayoutList inputLayoutList;
			uint32_t byteOffset = 0;
			for (const Technique::InputLayoutDescription& inputLayoutDesc : inputLayout)
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

			return D3D12Technique::Ptr(new D3D12Technique(shaderInfo, inputLayout, rootSignatureDescription, d3d12RootSignature, std::move(shaderBlobs), std::move(inputLayoutList)));
		}

		PipelineStateObject::Ptr D3D12Context::CreatePSO(const PipelineStateObject::Description& description)
		{
			D3D12Technique* d3d12Technique = static_cast<D3D12Technique*>(description.Technique.Get());

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
			ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

			psoDesc.InputLayout = { d3d12Technique->GetInputLayout().data(), (uint32_t)d3d12Technique->GetInputLayout().size() };
			psoDesc.pRootSignature = d3d12Technique->GetRootSignature().Get();
			if (d3d12Technique->GetShaderBlobs().contains(Technique::ShaderInfoDescription::ShaderType::VertexShader))
			{
				psoDesc.VS =
				{
					.pShaderBytecode = reinterpret_cast<uint8_t*>(d3d12Technique->GetShaderBlobs().at(Technique::ShaderInfoDescription::ShaderType::VertexShader)->GetBufferPointer()),
					.BytecodeLength = d3d12Technique->GetShaderBlobs().at(Technique::ShaderInfoDescription::ShaderType::VertexShader)->GetBufferSize()
				};
			}
			if (d3d12Technique->GetShaderBlobs().contains(Technique::ShaderInfoDescription::ShaderType::PixelShader))
			{
				psoDesc.PS =
				{
					.pShaderBytecode = reinterpret_cast<uint8_t*>(d3d12Technique->GetShaderBlobs().at(Technique::ShaderInfoDescription::ShaderType::PixelShader)->GetBufferPointer()),
					.BytecodeLength = d3d12Technique->GetShaderBlobs().at(Technique::ShaderInfoDescription::ShaderType::PixelShader)->GetBufferSize()
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

			return D3D12PipelineStateObject::Ptr(new D3D12PipelineStateObject(description, pso));
		}

		RefCountPtr<D3D12DescriptorHeap> D3D12Context::GetSrvHeap() const
		{
			return m_SrvCbvUavHeap;
		}

		void D3D12Context::SetDescriptorHeaps()
		{
			ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvCbvUavHeap->GetHandle().Get() };
			m_CommandList->GetHandle()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		}
	}
}
