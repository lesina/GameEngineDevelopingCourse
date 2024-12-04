#pragma once

#include <RHIBuffer.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHIMesh
		{
		public:
			using ID = uint64_t;
			using Ptr = RHIMesh*;

			struct VertexBufferDescription
			{
				uint32_t Count = 1;
				uint32_t ElementSize = 1;
				void* initData = nullptr;
			};

			struct IndexBufferDescription
			{
				uint32_t Count = 1;
				ResourceFormat Format = ResourceFormat::R16_UINT;
				void* initData = nullptr;
			};

		public:
			RHIMesh() = delete;
			RHIMesh(RHIBuffer::Ptr vertexBuffer, RHIBuffer::Ptr indexBuffer, ResourceFormat IndexFormat)
				: m_VertexBuffer(vertexBuffer)
				, m_IndexBuffer(indexBuffer)
				, m_IndexFormat(IndexFormat)
			{}

			const RHIBuffer::Ptr& GetVertexBuffer() const { return m_VertexBuffer; }
			const RHIBuffer::Ptr& GetIndexBuffer() const { return m_IndexBuffer; }
			const ResourceFormat& GetIndexFormat() const { return m_IndexFormat; }

		protected:
			RHIBuffer::Ptr m_VertexBuffer;
			RHIBuffer::Ptr m_IndexBuffer;
			ResourceFormat m_IndexFormat;
		};
	}
}