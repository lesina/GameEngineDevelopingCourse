#include <Quaternion.h>
#include <cmath>

namespace GameEngine::Math
{
    Quaternion Quaternion::FromAxisAngle(const Vector3f& axis, float angle)
    {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        return Quaternion(
            axis.x * s,
            axis.y * s,
            axis.z * s,
            std::cos(halfAngle)
        );
    }

    Quaternion Quaternion::FromEuler(float pitch, float yaw, float roll)
    {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    Quaternion Quaternion::operator*(const Quaternion& other) const
    {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Vector3f Quaternion::operator*(const Vector3f& v) const
    {
        Vector3f qv(x, y, z);
        Vector3f uv = qv.CrossProduct(v);
        Vector3f uuv = qv.CrossProduct(uv);
        return v + ((uv * w) + uuv) * 2.0f;
    }

    Quaternion Quaternion::Normalized() const
    {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        if (len > 0.0001f)
        {
            float invLen = 1.0f / len;
            return Quaternion(x * invLen, y * invLen, z * invLen, w * invLen);
        }
        return *this;
    }

    Matrix3x3f Quaternion::ToRotationMatrix() const
    {
        Matrix3x3f result;
        
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        result.SetElement(1.0f - 2.0f * (yy + zz), 0, 0);
        result.SetElement(2.0f * (xy - zw), 0, 1);
        result.SetElement(2.0f * (xz + yw), 0, 2);

        result.SetElement(2.0f * (xy + zw), 1, 0);
        result.SetElement(1.0f - 2.0f * (xx + zz), 1, 1);
        result.SetElement(2.0f * (yz - xw), 1, 2);

        result.SetElement(2.0f * (xz - yw), 2, 0);
        result.SetElement(2.0f * (yz + xw), 2, 1);
        result.SetElement(1.0f - 2.0f * (xx + yy), 2, 2);

        return result;
    }
} 