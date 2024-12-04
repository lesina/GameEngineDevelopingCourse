#include <RenderCore.h>

namespace GameEngine
{
	namespace RenderCore
	{
		std::unique_ptr<RenderSettings> g_RenderSettings = std::make_unique<RenderSettings>();
	}
}