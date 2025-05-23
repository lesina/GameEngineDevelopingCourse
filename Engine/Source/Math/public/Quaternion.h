#pragma once

#include <Math/export.h>
#include <Vector.h>
#include <Matrix.h>

namespace GameEngine
{
    namespace Math
    {
        class MATH_API Quaternion
        {
        public:
            float x, y, z, w;

            Quaternion() : x(0), y(0), z(0), w(1) {}
            Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

            static Quaternion FromAxisAngle(const Vector3f& axis, float angle);
            static Quaternion FromEuler(float pitch, float yaw, float roll);

            Quaternion operator*(const Quaternion& other) const;
            Vector3f operator*(const Vector3f& v) const;
            Quaternion Normalized() const;
            Matrix3x3f ToRotationMatrix() const;
        };
    }
} 