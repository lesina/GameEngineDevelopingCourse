#pragma once

#include <Vector.h>
#include <RenderEngine/export.h>
#include <RenderThread.h>

namespace GameEngine::Render
{
	class Mesh;
	class Material;
	class Geometry;

	namespace HAL
	{
		class RenderData;
	}

	class RENDER_ENGINE_API RenderObject final
	{
	public:
		RenderObject() = delete;
		RenderObject(HAL::RenderData* renderData);
		~RenderObject();

	public:
		HAL::RenderData* GetRenderData() const { return m_RenderData; }

		const Math::Vector3f& GetPosition(size_t frame) const;
		void SetPosition(Math::Vector3f position, size_t frame);

	protected:
		HAL::RenderData* m_RenderData;
	};
}