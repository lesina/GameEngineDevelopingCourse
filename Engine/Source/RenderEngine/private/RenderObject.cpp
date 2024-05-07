#include <RenderData.h>
#include <RenderObject.h>

namespace GameEngine::Render
{
	RenderObject::RenderObject(HAL::RenderData* renderData)
	{
		m_RenderData = renderData;

		for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
		{
			m_RenderData->SetPosition(Math::Vector3f::Zero(), i);
		}
	}

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
		m_RenderData->SetPosition(position, frame);
	}
}