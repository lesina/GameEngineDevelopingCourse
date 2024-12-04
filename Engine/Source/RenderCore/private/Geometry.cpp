#include <Geometry.h>

namespace GameEngine::RenderCore
{
	Geometry::Geometry(
		VertexType* vertices,
		uint32_t verticesCount,
		IndexType* indices,
		uint32_t indicesCount
	) : m_Vertices(vertices),
		m_Indices(indices),
		m_VerticesCount(verticesCount),
		m_IndicesCount(indicesCount)
	{
		assert(vertices != nullptr);
		assert(indices != nullptr);

		m_Vertices = new VertexType[verticesCount];
		m_Indices = new IndexType[indicesCount];

		std::memcpy(m_Vertices, vertices, verticesCount * sizeof(VertexType));
		std::memcpy(m_Indices, indices, indicesCount * sizeof(IndexType));
	}

	Geometry::~Geometry()
	{
		delete[] m_Indices;
		delete[] m_Vertices;
	}
}