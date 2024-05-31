#include <RenderData.h>
#include <RenderObject.h>

namespace GameEngine::Render
{
	RenderObject::~RenderObject()
	{
		delete m_RenderData;
	}

	const Math::Vector3f& RenderObject::GetPosition(size_t frame) const
	{
		return m_RenderData->GetPosition(frame);
	}

	void RenderObject::SetPosition(Math::Vector3f position, size_t frame)
	{
		if (m_RenderData != nullptr) [[unlikely]]
		{
			m_RenderData->SetPosition(position, frame);
		}
	}
}