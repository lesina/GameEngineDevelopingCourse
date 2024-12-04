#pragma once

#include <Vector.h>
#include <RenderEngine/export.h>
#include <RenderThread.h>
#include <Material.h>

// That's why macroses are bad
#ifdef max
#undef max
#endif

namespace GameEngine::Render
{
	class RENDER_ENGINE_API RenderObject final
	{
	public:
		using MeshID = uint64_t;

		constexpr static MeshID k_invalidMeshID = std::numeric_limits<MeshID>::max();
		constexpr static Material::ID k_invalidMaterialID = std::numeric_limits<Material::ID>::max();

	public:
		RenderObject() = delete;
		RenderObject(std::nullptr_t);
		RenderObject(MeshID meshID, Material::ID materialID);
		~RenderObject();

	public:
		const MeshID& GetMeshID() const { return m_MeshID; }
		const Material::ID& GetMaterialID() const { return m_MaterialID; }

		void SetMeshID(MeshID id) { assert(RenderThread::IsRenderThread()); m_MeshID = id; }
		void SetMaterialID(Material::ID id) { assert(RenderThread::IsRenderThread()); m_MaterialID = id; }

		const Math::Vector3f& GetPosition(size_t frame) const { return m_Position[frame]; }
		void SetPosition(Math::Vector3f position, size_t frame) { m_Position[frame] = position; }

	private:
		void Init();

	private:
		Math::Vector3f m_Position[RenderCore::g_FrameBufferCount];

		MeshID m_MeshID = k_invalidMeshID;
		Material::ID m_MaterialID = k_invalidMaterialID;
	};
}