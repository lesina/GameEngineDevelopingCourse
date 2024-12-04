# get all files
file(GLOB
	PRIVATE_FILES
	private/*.cpp
	private/*.h
)
file(GLOB
	PUBLIC_FILES
	public/*.h
)

# Add library
include(GenerateExportHeader)
add_library(RenderEngine SHARED)
generate_export_header(RenderEngine EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME RENDER_ENGINE_API)

set_target_properties(RenderEngine PROPERTIES FOLDER "Engine")

target_compile_definitions(RenderEngine
	PRIVATE
		RenderEngine_EXPORTS
		UNICODE)

# Set Sources
target_sources(RenderEngine
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(RenderEngine 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(RenderEngine PRIVATE
	Math
	RenderCore
	RHI
	Core
)

target_precompile_headers(RenderEngine
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
		<thread>
		<limits>
		<chrono>
		<filesystem>
		<bitset>
		<queue>
		<variant>
)