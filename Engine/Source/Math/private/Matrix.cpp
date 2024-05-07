#include <Matrix.h>
#include <Constants.h>
#include <MathHelper.h>

namespace GameEngine
{
	namespace Math
	{
		Matrix4x4f ViewMatrixLH(
			Vector3f pos,
			Vector3f viewDir,
			Vector3f up
		)
		{
			assert(viewDir != Vector3f::Zero());
			assert(up != Vector3f::Zero());

			viewDir = viewDir.Normalized();
			
			Vector3f side = up.CrossProduct(viewDir);
			side = side.Normalized();

			up = viewDir.CrossProduct(side);

			pos = -pos;

			float d0 = side * pos;
			float d1 = up * pos;
			float d2 = viewDir * pos;

			Matrix4x4f result;
			
			result.SetElement(side.x, 0, 0);
			result.SetElement(side.y, 1, 0);
			result.SetElement(side.z, 2, 0);
			result.SetElement(d0, 3, 0);

			result.SetElement(up.x, 0, 1);
			result.SetElement(up.y, 1, 1);
			result.SetElement(up.z, 2, 1);
			result.SetElement(d1, 3, 1);

			result.SetElement(viewDir.x, 0, 2);
			result.SetElement(viewDir.y, 1, 2);
			result.SetElement(viewDir.z, 2, 2);
			result.SetElement(d2, 3, 2);

			result.SetElement(0, 0, 3);
			result.SetElement(0, 1, 3);
			result.SetElement(0, 2, 3);
			result.SetElement(1, 3, 3);

			return result;
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
			CalculateSinCos(SinFov, CosFov, 0.5f * FovAngleY);

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