#pragma once

#include <RHI/RHI/export.h>

// That's why macroses are bad
#ifdef max
#undef max
#endif

namespace GameEngine::Render
{
	namespace HAL
	{
		class RHIMesh
		{
		public:
			using ID = size_t;

			constexpr static ID k_invalidMeshID = std::numeric_limits<size_t>::max();

		public:
			RHIMesh() = delete;
			RHIMesh(HAL::RHIMesh::ID id)
				: m_ID(id) {}

			HAL::RHIMesh::ID GetID() { return m_ID; }
			const HAL::RHIMesh::ID& GetID() const { return m_ID; }

		protected:
			HAL::RHIMesh::ID m_ID;
		};
	}
}