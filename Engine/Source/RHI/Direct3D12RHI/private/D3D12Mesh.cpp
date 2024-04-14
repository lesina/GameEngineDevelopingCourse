#include <D3D12Mesh.h>
#include <D3D12Util.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12Mesh::D3D12Mesh(
			ID3D12Device* device,
			ID3D12GraphicsCommandList* cmdList,
			void* vertices,
			uint16_t verticesCount,
			uint16_t vertexTypeSize,
			void* indices,
			uint16_t indicesCount,
			uint16_t indexTypeSize)
		{
			assert(device != nullptr);
			assert(cmdList != nullptr);
			assert(vertices != nullptr);
			assert(indices != nullptr);

			const uint32_t vbByteSize = verticesCount * vertexTypeSize;
			const uint32_t ibByteSize = indicesCount * indexTypeSize;

			HRESULT hr = D3DCreateBlob(vbByteSize, &m_vertexBufferCPU);
			assert(SUCCEEDED(hr));
			CopyMemory(m_vertexBufferCPU->GetBufferPointer(), vertices, vbByteSize);

			hr = D3DCreateBlob(ibByteSize, &m_indexBufferCPU);
			assert(SUCCEEDED(hr));
			CopyMemory(m_indexBufferCPU->GetBufferPointer(), indices, ibByteSize);

			m_vertexBufferGPU = D3D12Util::CreateDefaultBuffer(device,
				cmdList, vertices, vbByteSize, m_vertexBufferUploader);

			m_indexBufferGPU = D3D12Util::CreateDefaultBuffer(device,
				cmdList, indices, ibByteSize, m_indexBufferUploader);

			m_vertexByteStride = vertexTypeSize;
			m_vertexBufferByteSize = vbByteSize;
			m_indexFormat = DXGI_FORMAT_R16_UINT;
			m_indexBufferByteSize = ibByteSize;

			m_indexCount = indicesCount;
			m_startIndexLocation = 0;
			m_baseVertexLocation = 0;
		}
	}
}