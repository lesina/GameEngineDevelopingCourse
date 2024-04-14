#pragma once

namespace GameEngine::Render
{
	namespace HAL
	{
		class Mesh
		{
		public:
			using Ptr = std::shared_ptr<Mesh>;

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