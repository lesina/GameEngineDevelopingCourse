#include <RenderObject.h>
#include <Mesh.h>

namespace cqe::Render
{
	RenderObject::RenderObject(RHI::Mesh* mesh, Material* material)
		: m_Mesh(mesh)
		, m_Material(material)
	{
		Init();
	}

	RenderObject::RenderObject(std::nullptr_t)
	{
		Init();
	}

	void RenderObject::Init()
	{

	}

	RenderObject::~RenderObject()
	{
		delete m_Material;
	}
}