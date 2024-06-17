#pragma once

#include <FileSystem.h>

namespace GameEngine
{
	namespace ScriptSystem
	{
		class Script final
		{
		public:
			Script() = delete;
			Script(const std::string& path);
			~Script() = default;

		public:
			const std::string& GetPath() const { return m_Path; }

		private:
			std::string m_Path;
		};
	}
}