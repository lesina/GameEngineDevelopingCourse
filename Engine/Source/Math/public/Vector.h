#pragma once

#include <Math/export.h>

namespace GameEngine
{
	namespace Math
	{
		template <typename T> requires std::is_arithmetic_v<T>
		class Vector2 final
		{
		public:
			T x;
			T y;

			Vector2() = default;

			Vector2(const Vector2&) = default;
			Vector2& operator=(const Vector2&) = default;

			Vector2(Vector2&&) = default;
			Vector2& operator=(Vector2&&) = default;

			static Vector2<T> Zero() { return Vector2<T>((T)0, (T)0); }

			explicit constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}
			explicit constexpr Vector2(T vector[2]) : x(vector[0]), y(vector[1]) {}

			inline Vector2<T> operator-(Vector2<T> other)
			{
				Vector2<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				return result;
			}

			inline Vector2<T> operator-()
			{
				Vector2<T> result;
				result.x = -x;
				result.y = -y;
				return result;
			}

			inline bool operator!=(Vector2<T> other)
			{
				return x != other.x || y != other.y;
			}
		};

		template <typename T> requires std::is_arithmetic_v<T>
		class Vector3 final
		{
		public:
			T x;
			T y;
			T z;

			Vector3() = default;

			Vector3(const Vector3&) = default;
			Vector3& operator=(const Vector3&) = default;

			Vector3(Vector3&&) = default;
			Vector3& operator=(Vector3&&) = default;

			static Vector3<T> Zero() { return Vector3<T>((T)0, (T)0, (T)0); }

			explicit constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
			explicit constexpr Vector3(T vector[3]) : x(vector[0]), y(vector[1]), z(vector[2]) {}

			inline float GetLength() const
			{
				return sqrtf(x * x + y * y + z * z);
			}

			inline Vector3<T> Normalized() const
			{
				float length = GetLength();
				Vector3<T> result;

				if (length > 0)
				{
					length = 1.0f / length;
				}

				result.x = x * length;
				result.y = y * length;
				result.z = z * length;

				return result;
			}

			inline Vector3<T> operator+(Vector3<T> other)
			{
				Vector3<T> result;
				result.x = x + other.x;
				result.y = y + other.y;
				result.z = z + other.z;
				return result;
			}

			inline Vector3<T> operator-(Vector3<T> other)
			{
				Vector3<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				result.z = z - other.z;
				return result;
			}

			inline Vector3<T> operator-()
			{
				Vector3<T> result;
				result.x = -x;
				result.y = -y;
				result.z = -z;
				return result;
			}

			inline float operator*(Vector3<T> other)
			{
				return x * other.x + y * other.y + z * other.z;
			}

			inline Vector3<T> operator*(float scale)
			{
				return Vector3<T>(x * scale, y * scale, z * scale);
			}

			inline Vector3<T> CrossProduct(Vector3<T> other) const
			{
				return Vector3<T>(
					y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
			}

			inline bool operator!=(Vector3<T> other)
			{
				return x != other.x || y != other.y || z != other.z;
			}
		};

		template <typename T> requires std::is_arithmetic_v<T>
		class Vector4 final
		{
		public:
			T x;
			T y;
			T z;
			T w;

			Vector4() = default;

			Vector4(const Vector4&) = default;
			Vector4& operator=(const Vector4&) = default;

			Vector4(Vector4&&) = default;
			Vector4& operator=(Vector4&&) = default;

			static Vector4<T> Zero() { return Vector4<T>((T)0, (T)0, (T)0, (T)0); }

			explicit constexpr Vector4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
			explicit constexpr Vector4(T vector[4]) : x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3]) {}

			inline Vector4<T> operator-(Vector4<T> other)
			{
				Vector4<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				result.z = z - other.z;
				result.w = w - other.w;
				return result;
			}

			inline bool operator!=(Vector4<T> other)
			{
				return x != other.x && y != other.y && z != other.z && w != other.w;
			}
		};

		using Vector2f = Vector2<float>;
		using Vector2i = Vector2<int>;

		using Vector3f = Vector3<float>;
		using Vector3i = Vector3<int>;

		using Vector4f = Vector4<float>;
		using Vector4i = Vector4<int>;
	}
}