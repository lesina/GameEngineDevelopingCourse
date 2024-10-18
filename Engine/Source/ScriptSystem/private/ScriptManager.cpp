#include <flecs_lua.h>
#include <lua.hpp>

#include <Debug/Console.h>
#include <FileSystem.h>
#include <Input/InputHandler.h>
#include <Script.h>
#include <ScriptManager.h>

namespace GameEngine::ScriptSystem
{
	ScriptManager::ScriptManager(flecs::world& world)
	{
		m_World = world.get_world();

		/* Creates a script host for the world */
		ECS_IMPORT(world, FlecsLua);

		lua_State* L = ecs_lua_get_state(m_World);

		luaL_openlibs(L);

		AddScript("Physics.lua");
		AddScript("DeathTimer.lua");
		AddScript("Collision.lua");
	}

	ScriptManager::~ScriptManager()
	{
		for (Script* script : m_ScriptList)
		{
			delete script;
		}
	}

	void ScriptManager::AddScript(const std::string& scriptPath)
	{
		lua_State* L = ecs_lua_get_state(m_World);

		std::string path = Core::g_FileSystem->GetScriptsPath(scriptPath).string();
		Script* script = new Script(path);
		m_ScriptList.push_back(script);

		ExecuteScript(path.c_str());
	}

	void ScriptManager::Update(float dt)
	{
		if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::F2)) [[unlikely]]
		{
			ReloadScripts();
		}
	}

	void ScriptManager::ReloadScripts()
	{
#if defined(DEBUG)
		for (Script* script : m_ScriptList)
		{
			ExecuteScript(script->GetPath().c_str());
		}
#endif
	}

	void ScriptManager::ExecuteScript(const char* scriptPath)
	{
		lua_State* L = ecs_lua_get_state(m_World);

		int ret = luaL_dofile(L, scriptPath);

		if (ret)
		{
			const char* err = lua_tostring(L, lua_gettop(L));
			Core::Console::PrintDebug(err);
			ecs_os_err(NULL, 0, err);
		}
	}
}