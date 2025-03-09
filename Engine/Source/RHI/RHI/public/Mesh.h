#pragma once

#include <Buffer.h>

namespace cqe::Render
{
	namespace RHI
	{
		// This part of the code is a replacement for the MeshManager that we are going to develop later
		static uint64_t s_GlobalMeshCounter = 0;

		class Mesh
		{
		public:
			using ID = uint64_t;

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
			Mesh() = delete;
			Mesh(Buffer::Ptr vertexBuffer, Buffer::Ptr indexBuffer, ResourceFormat IndexFormat)
				: m_VertexBuffer(vertexBuffer)
				, m_IndexBuffer(indexBuffer)
				, m_IndexFormat(IndexFormat)
				, m_ID(s_GlobalMeshCounter++)
			{}

			const Buffer::Ptr& GetVertexBuffer() const { return m_VertexBuffer; }
			const Buffer::Ptr& GetIndexBuffer() const { return m_IndexBuffer; }
			const ResourceFormat& GetIndexFormat() const { return m_IndexFormat; }
			const ID GetID() const { return m_ID; }

		protected:
			Buffer::Ptr m_VertexBuffer;
			Buffer::Ptr m_IndexBuffer;
			ResourceFormat m_IndexFormat;

		protected:
			// This part of the code is a replacement for the MeshManager that we are going to develop later
			ID m_ID = 0;
		};

	}
}