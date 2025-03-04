#pragma once

#include <array.h>
#include <RenderCore/export.h>
#include <Vector.h>

namespace cqe
{
	namespace RenderCore
	{
		class RENDER_CORE_API Geometry final
		{
		public:
			// IndexType should be consistent with index format
			// DXGI_FORMAT_R16_UINT
			using IndexType = uint16_t;

			struct Vertex
			{
				Math::Vector3f m_Pos = Math::Vector3f{ 0.0f, 0.0f, 0.0f };
				Math::Vector2f m_Uv = Math::Vector2f{ 0.0f, 0.0f };
			};
			using VertexType = Vertex;
			using Ptr = std::shared_ptr<Geometry>;

		public:
			Geometry() = delete;
			Geometry(
				VertexType* vertices,
				uint32_t verticesCount,
				IndexType* indices,
				uint32_t indicesCount);

			~Geometry();

		public:
			VertexType* GetVertices() const { return m_Vertices; }
			IndexType* GetIndices() const { return m_Indices; }

			uint32_t GetVertexCount() const { return m_VerticesCount; }
			uint32_t GetIndexCount() const { return m_IndicesCount; }

		protected:
			VertexType* m_Vertices;
			IndexType* m_Indices;

			// Put it in this order to save some memory
			uint32_t m_VerticesCount;
			uint32_t m_IndicesCount;
		};
	}
}