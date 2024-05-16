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
		Math::Vector3f right = GetRightDir();
		Math::Vector3f up = m_ViewDir.CrossProduct(right).Normalized();

		Math::Matrix3x3f rotationMatrixX = Math::GetRotationMatrix(up, dx);
		Math::Matrix3x3f rotationMatrixY = Math::GetRotationMatrix(right, dy);

		m_ViewDir = rotationMatrixX * rotationMatrixY * m_ViewDir;
		m_ViewDir = m_ViewDir.Normalized();
	}

	void Camera::Move(Math::Vector3f dir)
	{
		m_CurrentMoveDir = m_CurrentMoveDir + dir;
	}

	void Camera::Update(float dt)
	{
		m_Position = m_Position + m_CurrentMoveDir.Normalized() * m_MovementSpeed * dt;
		m_CurrentMoveDir = Math::Vector3f::Zero();
	}

	Math::Vector3f Camera::GetRightDir() const
	{
		return Math::Vector3f(0.0f, 1.0f, 0.0f).CrossProduct(m_ViewDir).Normalized();
	}
}