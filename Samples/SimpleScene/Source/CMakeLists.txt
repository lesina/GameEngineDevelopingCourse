# get all files
file(GLOB_RECURSE
	PRIVATE_FILES
	private/*.h
	private/*.cpp
)

file(GLOB_RECURSE
	PUBLIC_FILES
	public/*.h
)

# Add library
include(GenerateExportHeader)
add_library(${PROJECT_NAME} SHARED)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME GAME_API)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Game")

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		${PROJECT_NAME}_EXPORTS
		UNICODE)

# Set Sources
target_sources(${PROJECT_NAME}
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(${PROJECT_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Core
	EntitySystem
	Math
	RenderCore
	RenderEngine
	ScriptSystem
)

# Defining it here so the Engine's build system stay clear
target_link_libraries(Game PRIVATE
	${PROJECT_NAME}
)

target_precompile_headers(${PROJECT_NAME}
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
)