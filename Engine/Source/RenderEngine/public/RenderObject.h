#pragma once

#include <RenderEngine/export.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class Mesh;
		class Material;
	}

	class RENDER_ENGINE_API RenderObject final
	{
	public:
		using Ptr = std::shared_ptr<RenderObject>;

	public:
		RenderObject();

	public:
		std::shared_ptr<HAL::Mesh> m_mesh;
		std::shared_ptr<HAL::Material> m_material;
	};
}