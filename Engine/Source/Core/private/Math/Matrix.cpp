#include <Math/Matrix.h>

namespace GameEngine::Core
{
	namespace Math
	{
		Matrix4x4f ViewMatrixLH(
			Vector3f pos,
			Vector3f target,
			Vector3f up
		)
		{
			Vector3f eyeDirection = target - pos;

			assert(eyeDirection != Vector3f::Zero());
			assert(up != Vector3f::Zero());

			eyeDirection = eyeDirection.Normalized();
			
			Vector3f side = up.CrossProduct(eyeDirection);
			side = side.Normalized();

			up = eyeDirection.CrossProduct(side);

			pos = -pos;

			float d0 = side * pos;
			float d1 = up * pos;
			float d2 = eyeDirection * pos;

			Matrix4x4f result;
			
			result.SetElement(side.x, 0, 0);
			result.SetElement(side.y, 0, 1);
			result.SetElement(side.z, 0, 2);
			result.SetElement(d0, 0, 3);

			result.SetElement(up.x, 1, 0);
			result.SetElement(up.y, 1, 1);
			result.SetElement(up.z, 1, 2);
			result.SetElement(d1, 1, 3);

			result.SetElement(eyeDirection.x, 2, 0);
			result.SetElement(eyeDirection.y, 2, 1);
			result.SetElement(eyeDirection.z, 2, 2);
			result.SetElement(d2, 2, 3);

			result.SetElement(0, 3, 0);
			result.SetElement(0, 3, 1);
			result.SetElement(0, 3, 2);
			result.SetElement(1, 3, 3);

			result = result.Transpose();

			return result;
		}


		constexpr float XM_PI = 3.141592654f;
		constexpr float XM_2PI = 6.283185307f;
		constexpr float XM_1DIVPI = 0.318309886f;
		constexpr float XM_1DIV2PI = 0.159154943f;
		constexpr float XM_PIDIV2 = 1.570796327f;
		constexpr float XM_PIDIV4 = 0.785398163f;
		inline void XMScalarSinCos
		(
			float* pSin,
			float* pCos,
			float  Value
		) noexcept
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		Matrix4x4f ProjectionMatrixLH(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			float    SinFov;
			float    CosFov;
			XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix4x4f result;

			result.SetElement(Width, 0, 0);
			result.SetElement(0.0f, 0, 1);
			result.SetElement(0.0f, 0, 2);
			result.SetElement(0.0f, 0, 3);

			result.SetElement(0.0f, 1, 0);
			result.SetElement(Height, 1, 1);
			result.SetElement(0.0f, 1, 2);
			result.SetElement(0.0f, 1, 3);

			result.SetElement(0.0f, 2, 0);
			result.SetElement(0.0f, 2, 1);
			result.SetElement(fRange, 2, 2);
			result.SetElement(1.0f, 2, 3);

			result.SetElement(0.0f, 3, 0);
			result.SetElement(0.0f, 3, 1);
			result.SetElement(-fRange * NearZ, 3, 2);
			result.SetElement(0.0f, 3, 3);

			return result;
		}
	}
}