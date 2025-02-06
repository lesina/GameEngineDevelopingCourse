# get all files
file(GLOB
	PRIVATE_FILES
	private/*.h
	private/*.cpp
)
file(GLOB
	PUBLIC_FILES
	public/*.h
)

# Add library
include(GenerateExportHeader)
add_library(ScriptSystem SHARED)
generate_export_header(ScriptSystem EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME SCRIPT_SYSTEM_API)

set_target_properties(ScriptSystem PROPERTIES FOLDER "Engine")

#setting libs
if (WINDOWS)
	find_library(FLECS_DEBUG NAMES flecs
		PATHS ${CMAKE_LIBRARY_PATH}/flecs/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(FLECS_RELEASE NAMES flecs 
		PATHS ${CMAKE_LIBRARY_PATH}/flecs/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(FLECS_LIBRARIES debug ${FLECS_DEBUG} optimized ${FLECS_RELEASE})
	
	find_library(LUA_DEBUG NAMES lua
		PATHS ${CMAKE_LIBRARY_PATH}/lua/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(LUA_RELEASE NAMES lua 
		PATHS ${CMAKE_LIBRARY_PATH}/lua/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(LUA_LIBRARIES debug ${LUA_DEBUG} optimized ${LUA_RELEASE})
	
	find_library(FLECS_LUA_DEBUG NAMES flecsLua
		PATHS ${CMAKE_LIBRARY_PATH}/flecs-lua/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(FLECS_LUA_RELEASE NAMES flecsLua 
		PATHS ${CMAKE_LIBRARY_PATH}/flecs-lua/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(FLECS_LUA_LIBRARIES debug ${FLECS_LUA_DEBUG} optimized ${FLECS_LUA_RELEASE})
endif()

target_compile_definitions(ScriptSystem
	PRIVATE
		ScriptSystem_EXPORTS
		flecs_lua_STATIC
		UNICODE
)

# Set Sources
target_sources(ScriptSystem
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(ScriptSystem 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PUBLIC ${CMAKE_LIBRARY_PATH}/flecs/include
	PUBLIC ${CMAKE_LIBRARY_PATH}/lua/src
	PUBLIC ${CMAKE_LIBRARY_PATH}/flecs-lua/include
)

target_link_libraries(ScriptSystem PRIVATE
	Core
	Math
	RenderCore
	RenderEngine
	
	PUBLIC ${FLECS_LIBRARIES}
	PUBLIC ${LUA_LIBRARIES}
	PUBLIC ${FLECS_LUA_LIBRARIES}
)

target_precompile_headers(ScriptSystem
	PRIVATE
		<unordered_map>
		<vector>
		<string>
		<cassert>
		<format>
		<functional>
		<thread>
		<mutex>
		<condition_variable>
		<tuple>
		<utility>
		<cstdint>
		<limits>
		<chrono>
		<filesystem>
		<bitset>
		<queue>
		<stack>
)