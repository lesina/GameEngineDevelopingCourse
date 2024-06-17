#pragma once

#include <flecs.h>
#include <ScriptSystem/export.h>

namespace GameEngine
{
	namespace ScriptSystem
	{
		class Script;

		class SCRIPT_SYSTEM_API ScriptManager final
		{
		public:
			using Ptr = std::unique_ptr<ScriptManager>;
			using ScriptList = std::vector<Script*>;

		public:
			ScriptManager() = delete;
			ScriptManager(flecs::world& world);
			~ScriptManager();

		public:
			void AddScript(const std::string& scriptPath);
			void Update(float dt);
			void ReloadScripts();

		private:
			flecs::world m_World;

			ScriptList m_ScriptList;

		private:
			void ExecuteScript(const char* scriptPath);
		};
	}
}