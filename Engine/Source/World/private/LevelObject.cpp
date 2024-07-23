#include <LevelObject.h>

namespace GameEngine::World
{
	void LevelObject::AddComponent(const ComponentName& name, const ComponentDesc& desc)
	{
		m_ComponentList.push_back(std::make_pair(name, desc));
	}
}