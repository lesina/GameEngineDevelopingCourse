#include <D3D12Material.h>
#include <D3D12Util.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12Material::D3D12Material(
			const std::string& name,
			ID3D12Device* device,
			DXGI_FORMAT backBufferFormat,
			DXGI_FORMAT depthStencilFormat,
			bool b4xMsaaState,
			UINT u4xMsaaQuality
		)
			: Material(name)
		{
			BuildRootSignature(device);
			BuildShadersAndInputLayout(device);
			BuildPSO(device, backBufferFormat, depthStencilFormat, b4xMsaaState, u4xMsaaQuality);
		}

		void D3D12Material::BuildRootSignature(ID3D12Device* device)
		{
			// Shader programs typically require resources as input (constant buffers,
			// textures, samplers).  The root signature defines the resources the shader
			// programs expect.  If we think of the shader programs as a function, and
			// the input resources as function parameters, then the root signature can be
			// thought of as defining the function signature.  

			// Root parameter can be a table, root descriptor or root constants.
			CD3DX12_ROOT_PARAMETER slotRootParameter[1];

			// Create a single descriptor table of CBVs.
			CD3DX12_DESCRIPTOR_RANGE cbvTable;
			cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

			// A root signature is an array of root parameters.
			CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
			Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
				serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

			if (errorBlob != nullptr)
			{
				::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
			assert(SUCCEEDED(hr));

			hr = device->CreateRootSignature(
				0,
				serializedRootSig->GetBufferPointer(),
				serializedRootSig->GetBufferSize(),
				IID_PPV_ARGS(&m_RootSignature));

			assert(SUCCEEDED(hr));
		}

		void D3D12Material::BuildShadersAndInputLayout(ID3D12Device* device)
		{
			HRESULT hr = S_OK;

			m_vsByteCode = D3D12Util::CompileShader(L"..\\..\\..\\..\\..\\Assets\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
			m_psByteCode = D3D12Util::CompileShader(L"..\\..\\..\\..\\..\\Assets\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

			m_InputLayout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};
		}

		void D3D12Material::BuildPSO(
			ID3D12Device* device,
			DXGI_FORMAT backBufferFormat,
			DXGI_FORMAT depthStencilFormat,
			bool b4xMsaaState,
			UINT u4xMsaaQuality
		)
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
			psoDesc.RTVFormats[0] = backBufferFormat;
			psoDesc.SampleDesc.Count = b4xMsaaState ? 4 : 1;
			psoDesc.SampleDesc.Quality = b4xMsaaState ? (u4xMsaaQuality - 1) : 0;
			psoDesc.DSVFormat = depthStencilFormat;
			HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
			assert(SUCCEEDED(hr));
		}
	}
}