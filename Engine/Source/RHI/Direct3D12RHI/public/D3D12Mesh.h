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
#include <Mesh.h>
#include <Math/Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		struct Vertex
		{
			Core::Math::Vector3f Pos;
			Core::Math::Vector4f Color;
		};

		class D3D12Mesh final : public Mesh
		{
		public:
			D3D12Mesh() = delete;
			D3D12Mesh(
				ID3D12Device* device,
				ID3D12GraphicsCommandList* cmdList,
				void* vertices,
				uint16_t verticesCount,
				uint16_t vertexTypeSize,
				void* indices,
				uint16_t indicesCount,
				uint16_t indexTypeSize);

			D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
			{
				D3D12_VERTEX_BUFFER_VIEW vbv;
				vbv.BufferLocation = m_vertexBufferGPU->GetGPUVirtualAddress();
				vbv.StrideInBytes = m_vertexByteStride;
				vbv.SizeInBytes = m_vertexBufferByteSize;

				return vbv;
			}

			D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
			{
				D3D12_INDEX_BUFFER_VIEW ibv;
				ibv.BufferLocation = m_indexBufferGPU->GetGPUVirtualAddress();
				ibv.Format = m_indexFormat;
				ibv.SizeInBytes = m_indexBufferByteSize;

				return ibv;
			}

			void DisposeUploaders()
			{
				m_vertexBufferUploader = nullptr;
				m_indexBufferUploader = nullptr;
			}

			uint32_t GetIndexCount() const { return m_indexCount; }

		private:
			Microsoft::WRL::ComPtr<ID3DBlob> m_vertexBufferCPU = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> m_indexBufferCPU = nullptr;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferGPU = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBufferGPU = nullptr;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferUploader = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBufferUploader = nullptr;

			uint32_t m_vertexByteStride = 0;
			uint32_t m_vertexBufferByteSize = 0;
			DXGI_FORMAT m_indexFormat = DXGI_FORMAT_R16_UINT;
			uint32_t m_indexBufferByteSize = 0;

			uint32_t m_indexCount = 0;
			uint32_t m_startIndexLocation = 0;
			int m_baseVertexLocation = 0;

		};
	}
}