#pragma once

#include <Colors.h>
#include <Matrix.h>
#include <RHI/RHI/export.h>
#include <Vector.h>

// That's why macroses are bad
#ifdef max
#undef max
#endif

namespace GameEngine::Render
{
	namespace HAL
	{
		struct ObjectConstants
		{
			Math::Matrix4x4f WorldViewProj = Math::Matrix4x4f::Identity();
		};

		struct MaterialConstants
		{
			Math::Vector4f Color = Math::Vector4f::Zero();
		};

		class RHIMaterial
		{
		public:
			using ID = size_t;

			constexpr static ID k_invalidMaterialID = std::numeric_limits<size_t>::max();

		public:
			RHIMaterial() = delete;
			RHIMaterial(HAL::RHIMaterial::ID id)
				: m_ID(id) {}

			HAL::RHIMaterial::ID GetID() { return m_ID; }
			const HAL::RHIMaterial::ID& GetID() const { return m_ID; }

			RenderCore::Color GetAlbedo() const
			{
				return m_Albedo;
			}

		protected:
			HAL::RHIMaterial::ID m_ID = 0;

			RenderCore::Color m_Albedo = RenderCore::Colors::Red;
		};
	}
}