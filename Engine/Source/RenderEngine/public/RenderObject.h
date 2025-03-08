#pragma once

#include <Vector.h>
#include <RenderEngine/export.h>
#include <RenderThread.h>
#include <Material.h>

// That's why macroses are bad
#ifdef max
#undef max
#endif

namespace cqe::Render
{
	namespace RHI
	{
		class Mesh;
	}

	class RENDER_ENGINE_API RenderObject final
	{
	public:
		RenderObject() = delete;
		RenderObject(std::nullptr_t);
		RenderObject(RHI::Mesh* mesh, Material* material);
		~RenderObject();

	public:
		const RHI::Mesh* GetMesh() const { return m_Mesh; }
		const Material* GetMaterial() const { return m_Material; }

		void SetMesh(RHI::Mesh* mesh) { assert(RenderThread::IsRenderThread()); m_Mesh = mesh; }
		void SetMaterial(Material* material) { assert(RenderThread::IsRenderThread()); m_Material = material; }

		const Math::Vector3f& GetPosition() const
		{
			size_t frame = RenderThread::IsRenderThread() ? RenderThread::GetRenderFrame() : RenderThread::GetMainFrame();
			return m_Position[frame];
		}

		void SetPosition(Math::Vector3f position)
		{
			size_t frame = RenderThread::IsRenderThread() ? RenderThread::GetRenderFrame() : RenderThread::GetMainFrame();
			m_Position[frame] = position;
		}

	private:
		void Init();

	private:
		Math::Vector3f m_Position[RenderCore::g_FrameBufferCount];

		Material* m_Material = nullptr;
		RHI::Mesh* m_Mesh = nullptr;
	};
}