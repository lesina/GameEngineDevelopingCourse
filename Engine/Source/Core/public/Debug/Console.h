#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API Console final
		{
		public:
			template <typename... Args>
			static void PrintDebug(std::string_view fmtStr, Args&&... args)
			{
				PrintDebug(std::vformat(fmtStr, std::make_format_args(args...)));
			}

			static void PrintDebug(const std::string& message);
		};
	}
}