#pragma once

#include <Core/export.h>
#include <Math/Vector.h>

namespace GameEngine::Core
{
	namespace Math
	{
		using IndexType = uint8_t;

		template<typename T, IndexType width>
		class Vector;

		template <typename T, IndexType width, IndexType height> requires std::is_arithmetic_v<T>
		class Matrix final
		{
		public:
			T m_data[width * height] = {0};

			Matrix() = default;

			Matrix(const Matrix&) = default;
			Matrix& operator=(const Matrix&) = default;

			Matrix(Matrix&&) = default;
			Matrix& operator=(Matrix&&) = default;

		public:
			static inline Matrix<T, width, height> Identity()
			{
				static_assert(width == height);

				Matrix<T, width, height> matrix;

				for (int i = 0; i < height; ++i)
				{
					matrix.m_data[i * (width + 1)] = static_cast<T>(1);
				}

				return matrix;
			}

			inline Matrix<T, height, width> Transpose()
			{
				Matrix<T, height, width> matrix;

				for (IndexType row = 0; row < height; ++row)
				{
					for (IndexType column = 0; column < width; ++column)
					{
						matrix.SetElement(GetElement(row, column), column, row);
					}
				}

				return matrix;
			}

			template<IndexType newWidth>
			inline Matrix<T, newWidth, height> operator*(Matrix<T, newWidth, width> other)
			{
				Matrix<T, newWidth, height> matrix;

				for (IndexType row = 0; row < height; ++row)
				{
					for (IndexType column = 0; column < newWidth; ++column)
					{
						T resultValue = 0;

						for (IndexType index = 0; index < width; ++index)
						{
							resultValue += GetElement(row, index) * other.GetElement(index, column);
						}

						matrix.SetElement(resultValue, row, column);
					}
				}

				return matrix;
			}

			inline T GetElement(IndexType row, IndexType column)
			{
				return m_data[GetIndex(row, column)];
			}

			inline void SetElement(T value, IndexType row, IndexType column)
			{
				assert(row < height);
				assert(column < width);
				m_data[GetIndex(row, column)] = value;
			}

		protected:
			inline IndexType GetIndex(IndexType row, IndexType column)
			{
				assert(row < height);
				assert(column < width);
				return row * width + column;
			}
		};

		using Matrix4x4f = Matrix<float, 4, 4>;

		CORE_API Matrix4x4f ViewMatrixLH(
			Vector3f pos,
			Vector3f target,
			Vector3f up
		);

		CORE_API Matrix4x4f ProjectionMatrixLH(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		);
	}
}