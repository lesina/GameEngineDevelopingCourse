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
		Math::Quaternion yawQuat = Math::Quaternion::FromAxisAngle(Math::Vector3f(0.0f, 1.0f, 0.0f), yaw);
		Math::Quaternion pitchQuat = Math::Quaternion::FromAxisAngle(GetRightDir(), pitch);
		
		m_Rotation = yawQuat * pitchQuat * m_Rotation;
		m_Rotation = m_Rotation.Normalized();
		
		// Update view direction based on rotation
		m_ViewDir = m_Rotation * Math::Vector3f(0.0f, 0.0f, 1.0f);
		m_ViewDir = m_ViewDir.Normalized();
	}

	Math::Vector3f Camera::GetRightDir() const
	{
		return Math::Vector3f(0.0f, 1.0f, 0.0f).CrossProduct(m_ViewDir).Normalized();
	}
}