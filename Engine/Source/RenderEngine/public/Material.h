#pragma once

#include <RenderEngine/export.h>
#include <Colors.h>
#include <Matrix.h>
#include <Vector.h>

namespace GameEngine::Render
{
	class RENDER_ENGINE_API Material final
	{
	public:
		using ID = uint64_t;

	public:
		Material() = delete;
		Material(Material::ID id)
			: m_ID(id) {}

		Material::ID GetID() { return m_ID; }
		const Material::ID& GetID() const { return m_ID; }

		RenderCore::Color GetAlbedo() const
		{
			return m_Albedo;
		}

	protected:
		ID m_ID = 0;

		RenderCore::Color m_Albedo = RenderCore::Colors::Red;
	};
}