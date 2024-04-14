#pragma once

#include <Math/Matrix.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		struct ObjectConstants
		{
			Core::Math::Matrix4x4f WorldViewProj = Core::Math::Matrix4x4f::Identity();
		};

		class Material
		{
		public:
			using Ptr = std::shared_ptr<Material>;

			Material() = delete;
			Material(const std::string& name) : m_name(name) {}

		protected:
			// Give it a name so we can look it up by name.
			std::string m_name;

		public:
			const std::string& GetName() const
			{
				return m_name;
			}
		};
	}
}