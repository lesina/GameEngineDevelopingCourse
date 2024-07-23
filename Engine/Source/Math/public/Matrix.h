#pragma once

#include <Math/export.h>
#include <Vector.h>

namespace GameEngine
{
	namespace Math
	{
		using DimensionType = uint8_t;

		template<typename T, DimensionType width>
		class Vector;

		template <typename T, DimensionType width, DimensionType height> requires std::is_arithmetic_v<T>
		class Matrix final
		{
		public:
			Matrix() = default;

			Matrix(const Matrix&) = default;
			Matrix& operator=(const Matrix&) = default;

			Matrix(Matrix&&) = default;
			Matrix& operator=(Matrix&&) = default;

		public:
			static inline consteval Matrix<T, width, height> Identity() noexcept
			{
				static_assert(width == height);

				Matrix<T, width, height> matrix;

				for (int i = 0; i < height; ++i)
				{
					matrix.m_data[i * (width + 1)] = static_cast<T>(1);
				}

				return matrix;
			}

			inline Matrix<T, height, width> Transpose() const noexcept
			{
				Matrix<T, height, width> matrix;

				for (DimensionType row = 0; row < height; ++row)
				{
					for (DimensionType column = 0; column < width; ++column)
					{
						matrix.SetElement(GetElement(row, column), column, row);
					}
				}

				return matrix;
			}

			template<DimensionType newWidth>
			inline Matrix<T, newWidth, height> operator*(const Matrix<T, newWidth, width>& other) const noexcept
			{
				Matrix<T, newWidth, height> matrix;

				for (DimensionType row = 0; row < height; ++row)
				{
					for (DimensionType column = 0; column < newWidth; ++column)
					{
						T resultValue = 0;

						for (DimensionType index = 0; index < width; ++index)
						{
							resultValue += GetElement(row, index) * other.GetElement(index, column);
						}

						matrix.SetElement(resultValue, row, column);
					}
				}

				return matrix;
			}

			inline Vector3<T> operator*(const Vector3<T>& other) const noexcept
			{
				static_assert(width == 3, "Matrix and vector have different dimensions");
				Vector3<T> result;

				result.x = GetElement(0, 0) * other.x + GetElement(0, 1) * other.y + GetElement(0, 2) * other.z;
				result.y = GetElement(1, 0) * other.x + GetElement(1, 1) * other.y + GetElement(1, 2) * other.z;
				result.z = GetElement(2, 0) * other.x + GetElement(2, 1) * other.y + GetElement(2, 2) * other.z;

				return result;
			}

			inline const T& GetElement(DimensionType row, DimensionType column) const noexcept
			{
				return m_data[GetIndex(row, column)];
			}

			inline void SetElement(T value, DimensionType row, DimensionType column) noexcept
			{
				assert(row < height);
				assert(column < width);
				m_data[GetIndex(row, column)] = value;
			}

		protected:
			inline DimensionType GetIndex(DimensionType row, DimensionType column) const noexcept
			{
				assert(row < height);
				assert(column < width);
				return row * width + column;
			}

		protected:
			T m_data[width * height] = { 0 };
		};

		using Matrix4x4f = Matrix<float, 4, 4>;
		using Matrix3x3f = Matrix<float, 3, 3>;

		MATH_API Matrix4x4f ViewMatrixLH(
			Vector3f pos,
			Vector3f viewDir,
			Vector3f up
		);

		MATH_API Matrix4x4f ProjectionMatrixLH(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		);

		MATH_API Matrix3x3f GetRotationMatrix(Vector3f axis, float angle);
	}
}