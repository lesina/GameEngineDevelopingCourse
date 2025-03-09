#pragma once

#include <array.h>
#include <Geometry.h>
#include <Vector.h>

namespace cqe
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			Geometry* Cube()
			{
				constexpr Core::array<Geometry::VertexType, 8> vertices =
				{
					Geometry::VertexType{Math::Vector3f{ -1.0f, -1.0f, -1.0f }, Math::Vector2f{ 0.0f, 1.0f }},
					Geometry::VertexType(Math::Vector3f{ -1.0f, +1.0f, -1.0f }, Math::Vector2f{ 0.0f, 0.0f }),
					Geometry::VertexType(Math::Vector3f{ +1.0f, +1.0f, -1.0f }, Math::Vector2f{ 1.0f, 0.0f }),
					Geometry::VertexType(Math::Vector3f{ +1.0f, -1.0f, -1.0f }, Math::Vector2f{ 1.0f, 1.0f }),
					Geometry::VertexType(Math::Vector3f{ -1.0f, -1.0f, +1.0f }, Math::Vector2f{ 1.0f, 1.0f }),
					Geometry::VertexType(Math::Vector3f{ -1.0f, +1.0f, +1.0f }, Math::Vector2f{ 1.0f, 0.0f }),
					Geometry::VertexType(Math::Vector3f{ +1.0f, +1.0f, +1.0f }, Math::Vector2f{ 0.0f, 0.0f }),
					Geometry::VertexType(Math::Vector3f{ +1.0f, -1.0f, +1.0f }, Math::Vector2f{ 0.0f, 1.0f })
				};

				constexpr Core::array<Geometry::IndexType, 36> indices =
				{
					// front face
					0, 1, 2,
					0, 2, 3,

					// back face
					4, 6, 5,
					4, 7, 6,

					// left face
					4, 5, 1,
					4, 1, 0,

					// right face
					3, 2, 6,
					3, 6, 7,

					// top face
					1, 5, 6,
					1, 6, 2,

					// bottom face
					4, 0, 3,
					4, 3, 7
				};
				
				return new Geometry((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}
		}
	}
}