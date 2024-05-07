#pragma once

#include <Matrix.h>
#include <RenderCore.h>
#include <RHIMesh.h>
#include <RHIMaterial.h>
#include <RHI/RHI/export.h>
#include <Vector.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class RenderData final
		{
		public:
			RenderData() = delete;

			RenderData(HAL::RHIMesh::ID meshID, HAL::RHIMaterial::ID materialID)
			{
				m_Mesh = new RHIMesh(meshID);
				m_Material = new RHIMaterial(materialID);
			}

			~RenderData()
			{
				delete m_Mesh;
				delete m_Material;
			}

			RHIMesh* GetMesh() { return m_Mesh; }
			RHIMaterial* GetMaterial() { return m_Material; }
			const Math::Vector3f& GetPosition(size_t frame) const { return m_Position[frame]; }
			void SetPosition(Math::Vector3f position, size_t frame) { m_Position[frame] = position; }

		protected:
			Math::Vector3f m_Position[RenderCore::g_FrameBufferCount];

			RHIMesh* m_Mesh;
			RHIMaterial* m_Material;
		};
	}
}