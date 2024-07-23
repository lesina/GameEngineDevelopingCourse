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

			static inline consteval Vector2<T> Zero() noexcept { return Vector2<T>((T)0, (T)0); }

			explicit constexpr Vector2(T _x, T _y) noexcept : x(_x), y(_y) {}
			explicit constexpr Vector2(const T (&vector)[2]) noexcept : x(vector[0]), y(vector[1]) {}

			inline Vector2<T> operator-(const Vector2<T>& other) const noexcept
			{
				Vector2<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				return result;
			}

			inline Vector2<T> operator-() const noexcept
			{
				Vector2<T> result;
				result.x = -x;
				result.y = -y;
				return result;
			}

			inline bool operator!=(const Vector2<T>& other) const noexcept
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

			static inline consteval Vector3<T> Zero() noexcept { return Vector3<T>((T)0, (T)0, (T)0); }

			explicit constexpr Vector3(T _x, T _y, T _z) noexcept : x(_x), y(_y), z(_z) {}
			explicit constexpr Vector3(const T (&vector)[3]) noexcept : x(vector[0]), y(vector[1]), z(vector[2]) {}

			inline float GetLength() const noexcept
			{
				return sqrtf(x * x + y * y + z * z);
			}

			inline Vector3<T> Normalized() const noexcept
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

			inline Vector3<T> operator+(const Vector3<T>& other) noexcept
			{
				Vector3<T> result;
				result.x = x + other.x;
				result.y = y + other.y;
				result.z = z + other.z;
				return result;
			}

			inline Vector3<T> operator-(const Vector3<T>& other) noexcept
			{
				Vector3<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				result.z = z - other.z;
				return result;
			}

			inline Vector3<T> operator-() noexcept
			{
				Vector3<T> result;
				result.x = -x;
				result.y = -y;
				result.z = -z;
				return result;
			}

			inline float operator*(const Vector3<T>& other) const noexcept
			{
				return x * other.x + y * other.y + z * other.z;
			}

			inline Vector3<T> operator*(float scale) const noexcept
			{
				return Vector3<T>(x * scale, y * scale, z * scale);
			}

			// We have left hand coordinate system, so use left hand to determine ther resulted vector
			inline Vector3<T> CrossProduct(const Vector3<T>& other) const noexcept
			{
				return Vector3<T>(
					y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
			}

			inline bool operator!=(const Vector3<T>& other) const noexcept
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

			static inline consteval Vector4<T> Zero() noexcept { return Vector4<T>((T)0, (T)0, (T)0, (T)0); }

			explicit constexpr Vector4(T _x, T _y, T _z, T _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
			explicit constexpr Vector4(const T (&vector)[4]) noexcept : x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3]) {}

			inline Vector4<T> operator-(const Vector4<T>& other) const noexcept
			{
				Vector4<T> result;
				result.x = x - other.x;
				result.y = y - other.y;
				result.z = z - other.z;
				result.w = w - other.w;
				return result;
			}

			inline bool operator!=(const Vector4<T>& other) const noexcept
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