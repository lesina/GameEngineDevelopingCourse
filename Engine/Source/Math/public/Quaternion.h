#pragma once

#include <Math/export.h>
#include <Vector.h>
#include <Matrix.h>

namespace GameEngine
{
    namespace Math
    {
        template<typename T> requires std::is_arithmetic_v<T>
        class Quaternion {
        public:
            // Maybe these shouldn't be public?
            T x;
            T y;
            T z;
            T w;  // w is the scalar part.

            Quaternion<T>() = default;
            Quaternion<T>(const Quaternion<T>& other) = default;
            Quaternion<T>& operator=(const Quaternion<T>& other) = default;

            Quaternion<T>(Quaternion<T>&& other) = default;
            Quaternion<T>& operator=(Quaternion<T>&& other) = default;

            static consteval inline Quaternion<T> Identity() noexcept
            {
                return Quaternion <T>(0, 0, 0, 1);
            }

            explicit constexpr inline Quaternion<T>(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}

            // `angle` is specified in radians. `axis` is not automatically normalized.
            static constexpr inline Quaternion<T> FromAxisAngle(const Vector3<T>& axis, const T angle) noexcept
            {
                T sine = static_cast<T>(sin(angle / 2.0));
                T cosine = static_cast<T>(cos(angle / 2.0));
                return Quaternion<T>(axis.x * sine, axis.y * sine, axis.z * sine, cosine);
            }

            // `angle` is specified in radians. `axis` is not automatically normalized.
            static constexpr inline Quaternion<T> FromAxisAngle(Vector3<T>&& axis, const T angle) noexcept
            {
                return FromAxisAngle(axis, angle);
            }

            // Doesn't check for zero.
            constexpr Quaternion<T> Normalized() const
            {
                double norm = x * x + y * y + z * z + w * w;
                return (*this) * static_cast<T>(1.0 / norm);
            }

            // Doesn't check for zero.
            constexpr Quaternion<T> Inverse() const
            {
                double norm = x * x + y * y + z * z + w * w;
                return Quaternion<T>(-x, -y, -z, w) * static_cast<T>(1.0 / norm);
            }

            constexpr Quaternion<T> operator*(const Quaternion<T>& other) const noexcept
            {
                return Quaternion<T>(
                    w * other.x + x * other.w + y * other.z - z * other.y,
                    w * other.y - x * other.z + y * other.w + z * other.x,
                    w * other.z + x * other.y - y * other.x + z * other.w,
                    w * other.w - x * other.x - y * other.y - z * other.z);
            }
    
            // Doesn't check if the quaternion is normalized.
            // Only accepts unit (normalized) quaternions.
            constexpr Vector3<T> RotateVector(const Vector3<T>& vector) const noexcept
            {
                Quaternion<T> result(vector.x, vector.y, vector.z, 0);
                result = (*this) * result * Quaternion<T>(-x, -y, -z, w);
                return Vector3<T>(result.x, result.y, result.z);
            }

            // Doesn't clamp t.
            constexpr static Quaternion<T> Lerp(const Quaternion<T>& a, const Quaternion<T>& b, T t) noexcept
            {
                return ((b - a) * t + a).Normalized();
            }

            constexpr T Dot(const Quaternion<T>& other) const noexcept
            {
                return x * other.x + y * other.y + z * other.z + w * other.w;
            }

            constexpr static Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t) noexcept
            {
                T omega = arcsin(a.Dot(b));
                return a * static_cast<T>(sin(omega * (1.0 - t))) + b * static_cast<T>(sin(omega * t));
            }

            // The quaternion should be normalized!
            constexpr Matrix<T, 4, 4> ToMatrix() const noexcept
            {
                Matrix<T, 4, 4> result = Matrix<T, 4, 4>::Identity();
                result.SetElement(1.0 - 2.0 * y * y - 2.0 * z * z, 0, 0);
                result.SetElement(2.0 * x * y - 2.0 * z * w, 1, 0);
                result.SetElement(2.0 * x * z + 2.0 * y * w, 2, 0);

                result.SetElement(2.0 * x * y + 2.0 * z * w, 0, 1);
                result.SetElement(1.0 - 2.0 * x * x - 2.0 * z * z, 1, 1);
                result.SetElement(2.0 * y * z - 2.0 * x * w, 2, 1);

                result.SetElement(2.0 * x * z - 2.0 * y * w, 0, 2);
                result.SetElement(2.0 * y * z + 2.0 * x * w, 1, 2);
                result.SetElement(1.0 - 2.0 * x * x - 2.0 * y * y, 2, 2);
                return result;
            }
        };

        template<typename T>
        constexpr inline Quaternion<T> operator*(const Quaternion<T>& quaternion, T scalar) noexcept
        {
            return Quaternion<T>(quaternion.x * scalar, quaternion.y * scalar, quaternion.z * scalar, quaternion.w * scalar);
        }

        template<typename T>
        constexpr Quaternion<T> operator+(const Quaternion<T>& a, const Quaternion<T>& b) noexcept
        {
            return Quaternion<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }

        template<typename T>
        constexpr Quaternion<T> operator+(Quaternion<T>&& a, Quaternion<T>&& b) noexcept
        {
            return a + b;
        }

        template<typename T>
        constexpr Quaternion<T> operator-(const Quaternion<T>& a, const Quaternion<T>& b) noexcept
        {
            return Quaternion<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
        }

        template<typename T>
        constexpr Quaternion<T> operator-(Quaternion<T>&& a, Quaternion<T>&& b) noexcept
        {
            return a - b;
        }
    }
}
