#pragma once

#include <array.h>
#include <Geometry.h>
#include <Vector.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			inline Geometry* Cube()
			{
				constexpr Core::array<Geometry::VertexType, 8> vertices =
				{
					Math::Vector3f(-1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, -1.0f, +1.0f),
					Math::Vector3f(-1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, -1.0f, +1.0f)
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

			inline Geometry* Bullet()
			{
				const int latitudeSegments = 16;
				const int longitudeSegments = 16;
				const float radius = 1.0f;

				std::vector<Geometry::VertexType> vertices;
				std::vector<Geometry::IndexType> indices;

				for (int lat = 0; lat <= latitudeSegments; ++lat)
				{
					float theta = lat * static_cast<float>(M_PI) / latitudeSegments;
					float sinTheta = std::sin(theta);
					float cosTheta = std::cos(theta);

					for (int lon = 0; lon <= longitudeSegments; ++lon)
					{
						float phi = lon * 2.0f * static_cast<float>(M_PI) / longitudeSegments;
						float sinPhi = std::sin(phi);
						float cosPhi = std::cos(phi);

						Math::Vector3f position = Math::Vector3f(
							radius * cosPhi * sinTheta,
							radius * cosTheta,
							radius * sinPhi * sinTheta
						);

						vertices.push_back(position);
					}
				}

				for (int lat = 0; lat < latitudeSegments; ++lat)
				{
					for (int lon = 0; lon < longitudeSegments; ++lon)
					{
						int first = (lat * (longitudeSegments + 1)) + lon;
						int second = first + longitudeSegments + 1;

						indices.push_back(first);
						indices.push_back(second);
						indices.push_back(first + 1);

						indices.push_back(second);
						indices.push_back(second + 1);
						indices.push_back(first + 1);
					}
				}

				return new Geometry(vertices.data(), vertices.size(), indices.data(), indices.size());
			}
		}
	}
}