#include <Camera.h>

namespace GameEngine::Core
{
	Camera* g_MainCamera = nullptr;

	Math::Matrix4x4f Camera::GetViewMatrix()
	{
		return Math::ViewMatrixLH(m_Position, m_ViewDir, Math::Vector3f(0.0f, 1.0f, 0.0f));
	}

	void Camera::Rotate(float yaw, float pitch)
	{
		Math::Vector3f right = GetRightDir();
		Math::Vector3f up = m_ViewDir.CrossProduct(right).Normalized();

		Math::Matrix3x3f rotationMatrixX = Math::GetRotationMatrix(up, yaw);
		Math::Matrix3x3f rotationMatrixY = Math::GetRotationMatrix(right, pitch);

		m_ViewDir = rotationMatrixX * rotationMatrixY * m_ViewDir;
		m_ViewDir = m_ViewDir.Normalized();
	}

	Math::Vector3f Camera::GetRightDir() const
	{
		return Math::Vector3f(0.0f, 1.0f, 0.0f).CrossProduct(m_ViewDir).Normalized();
	}
}