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
#include <Material.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12Material final : public Material
		{
		public:
			D3D12Material() = delete;
			D3D12Material(
				const std::string& name,
				ID3D12Device* device,
				DXGI_FORMAT backBufferFormat,
				DXGI_FORMAT depthStencilFormat,
				bool b4xMsaaState,
				UINT u4xMsaaQuality);

			Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPSO() const { return m_PSO; }
			Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() const { return m_RootSignature; }

		private:
			// These functions are temporal
			// They basically hardcode only one renderpass
			void BuildRootSignature(ID3D12Device* device);
			void BuildShadersAndInputLayout(ID3D12Device* device);
			void BuildPSO(
				ID3D12Device* device,
				DXGI_FORMAT backBufferFormat,
				DXGI_FORMAT depthStencilFormat,
				bool b4xMsaaState,
				UINT u4xMsaaQuality);

		private:
			Microsoft::WRL::ComPtr<ID3DBlob> m_vsByteCode = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> m_psByteCode = nullptr;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO = nullptr;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;

			std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
		};
	}
}