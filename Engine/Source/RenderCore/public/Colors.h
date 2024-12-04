#pragma once

#include <RenderCore/export.h>
#include <Vector.h>

namespace GameEngine
{
	namespace RenderCore
	{
		using Color = Math::Vector4f;

		namespace Colors
		{
			RENDER_CORE_API extern Color Red;
			RENDER_CORE_API extern Color Blue;
			RENDER_CORE_API extern Color LightSteelBlue;
		}
	}
}