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
add_library(RenderCore SHARED)
generate_export_header(RenderCore EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME RENDER_CORE_API)

set_target_properties(RenderCore PROPERTIES FOLDER "Engine")

target_compile_definitions(RenderCore
	PRIVATE
		RenderCore_EXPORTS
		UNICODE)

# Set Sources
target_sources(RenderCore
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(RenderCore 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(RenderCore PRIVATE
	Math
	Core
)

target_precompile_headers(RenderCore
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
)