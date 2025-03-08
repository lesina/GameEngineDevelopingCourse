#pragma once

#include <RenderEngine/export.h>
#include <Colors.h>
#include <Matrix.h>
#include <Vector.h>

namespace cqe::Render
{
	namespace RHI
	{
		class Texture;
	}

	// This part of the code is a replacement for the MaterialManager that we are going to develop later
	static uint64_t s_GlobalMaterialCounter = 0;

	class RENDER_ENGINE_API Material final
	{
	public:
		using ID = uint64_t;

	public:
		Material() = delete;
		Material(RHI::Texture* texture) 
			: m_ID(s_GlobalMaterialCounter++),
			  m_Texture(texture) {}

		~Material() { delete m_Texture; }

		Material::ID GetID() { return m_ID; }
		const Material::ID& GetID() const { return m_ID; }

		RenderCore::Color GetAlbedo() const
		{
			return m_Albedo;
		}

		RHI::Texture* GetTexture() const { return m_Texture; }

	protected:
		// This part of the code is a replacement for the MaterialManager that we are going to develop later
		ID m_ID = 0;

		RenderCore::Color m_Albedo = RenderCore::Colors::Red;
		RHI::Texture* m_Texture;
	};
}