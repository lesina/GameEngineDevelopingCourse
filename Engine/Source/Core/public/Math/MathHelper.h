#pragma once

#include <Math/Constants.h>

namespace GameEngine::Core
{
	namespace Math
	{
		inline void CalculateSinCos
		(
			float& pSin,
			float& pCos,
			const float Value
		) noexcept
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::INV_2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - 2 * Constants::PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > Constants::PI / 2)
			{
				y = Constants::PI - y;
				sign = -1.0f;
			}
			else if (y < -Constants::PI / 2)
			{
				y = -Constants::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			pCos = sign * p;
		}
	}
}