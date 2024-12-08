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

		Math::Quaternion<float> rotation = Math::Quaternion<float>::FromAxisAngle(up, yaw);
		rotation = Math::Quaternion<float>::FromAxisAngle(right, pitch) * rotation;

		m_ViewDir = rotation.RotateVector(m_ViewDir);
		m_ViewDir = m_ViewDir.Normalized();
	}

	Math::Vector3f Camera::GetRightDir() const
	{
		return Math::Vector3f(0.0f, 1.0f, 0.0f).CrossProduct(m_ViewDir).Normalized();
	}
}