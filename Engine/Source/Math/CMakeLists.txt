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
add_library(Math SHARED)
generate_export_header(Math EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME MATH_API)

target_compile_definitions(Math
	PRIVATE
		Math_EXPORTS
		UNICODE)

# Set Sources
target_sources(Math
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(Math 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

# target_link_libraries(Math PRIVATE
	# Core
# )

target_precompile_headers(Math
	PRIVATE
		<unordered_map>
		<vector>
		<string>
		<cassert>
		<format>
		<functional>
		<mutex>
		<condition_variable>
		<tuple>
		<utility>
		<cstdint>
		<limits>
		<chrono>
		<filesystem>
)