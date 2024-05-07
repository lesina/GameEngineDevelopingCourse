#include <Camera.h>

namespace GameEngine::Core
{
	Camera* g_MainCamera = nullptr;

	Math::Matrix4x4f Camera::GetViewMatrix()
	{
		return Math::ViewMatrixLH(m_Position, m_ViewDir, Math::Vector3f(0.0f, 1.0f, 0.0f));
	}

	void Camera::Rotate(float dx, float dy)
	{
        Math::Vector3f up = Math::Vector3f(0.0f, 1.0f, 0.0f);
        Math::Vector3f right = m_ViewDir.CrossProduct(up).Normalized();
        up = right.CrossProduct(m_ViewDir).Normalized();

        float cosX = std::cos(dx);
        float sinX = std::sin(dx);
        float cosY = std::cos(dy);
        float sinY = std::sin(dy);

        Math::Matrix3x3f rotationMatrixX;

        rotationMatrixX.SetElement(cosX + up.x * up.x * (1 - cosX), 0, 0);
        rotationMatrixX.SetElement(up.x * up.y * (1 - cosX) - up.z * sinX, 0, 1);
        rotationMatrixX.SetElement(up.x * up.y * (1 - cosX) + up.y * sinX, 0, 2);

        rotationMatrixX.SetElement(up.x * up.y * (1 - cosX) + up.z * sinX, 1, 0);
        rotationMatrixX.SetElement(cosX + up.y * up.y * (1 - cosX), 1, 1);
        rotationMatrixX.SetElement(up.y * up.z * (1 - cosX) - up.x * sinX, 1, 2);

        rotationMatrixX.SetElement(up.x * up.z * (1 - cosX) - up.y * sinX, 2, 0);
        rotationMatrixX.SetElement(up.y * up.z * (1 - cosX) + up.x * sinX, 2, 1);
        rotationMatrixX.SetElement(cosX + up.z * up.z * (1 - cosX), 2, 2);

        Math::Matrix3x3f rotationMatrixY;

        rotationMatrixY.SetElement(cosY + right.x * right.x * (1 - cosY), 0, 0);
        rotationMatrixY.SetElement(right.x * right.y * (1 - cosY) - right.z * sinY, 0, 1);
        rotationMatrixY.SetElement(right.x * right.y * (1 - cosY) + right.y * sinY, 0, 2);

        rotationMatrixY.SetElement(right.x * right.y * (1 - cosY) + right.z * sinY, 1, 0);
        rotationMatrixY.SetElement(cosY + right.y * right.y * (1 - cosY), 1, 1);
        rotationMatrixY.SetElement(right.y * right.z * (1 - cosY) - right.x * sinY, 1, 2);

        rotationMatrixY.SetElement(right.x * right.z * (1 - cosY) - right.y * sinY, 2, 0);
        rotationMatrixY.SetElement(right.y * right.z * (1 - cosY) + right.x * sinY, 2, 1);
        rotationMatrixY.SetElement(cosY + right.z * right.z * (1 - cosY), 2, 2);

        m_ViewDir = rotationMatrixX * rotationMatrixY * m_ViewDir;
        m_ViewDir = m_ViewDir.Normalized();
	}
}