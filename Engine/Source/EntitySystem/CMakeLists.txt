# get all files
file(GLOB
	PRIVATE_FILES
	private/*.h
	private/*.cpp
	private/ECS/*.h
	private/ECS/*.cpp
)
file(GLOB
	PUBLIC_FILES
	public/*.h
	public/ECS/*.h
)

# Add library
include(GenerateExportHeader)
add_library(EntitySystem SHARED)
generate_export_header(EntitySystem EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME ENTITY_SYSTEM_API)

set_target_properties(EntitySystem PROPERTIES FOLDER "Engine")

#setting flecs libs
if (WINDOWS)
	find_library(FLECS_DEBUG NAMES flecs
		PATHS ${CMAKE_LIBRARY_PATH}/flecs/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(FLECS_RELEASE NAMES flecs 
		PATHS ${CMAKE_LIBRARY_PATH}/flecs/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(FLECS_LIBRARIES debug ${FLECS_DEBUG} optimized ${FLECS_RELEASE})
endif()

target_compile_definitions(EntitySystem
	PRIVATE
		EntitySystem_EXPORTS
		UNICODE)

# Set Sources
target_sources(EntitySystem
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(EntitySystem 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PUBLIC ${CMAKE_LIBRARY_PATH}/flecs/include
)

target_link_libraries(EntitySystem PRIVATE
	Core
	Math
	RenderCore
	RenderEngine
	
	PUBLIC ${FLECS_LIBRARIES}
)

target_precompile_headers(EntitySystem
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
		<semaphore>
)