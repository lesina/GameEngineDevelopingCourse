# get all files
file(GLOB
	PRIVATE_FILES
	private/*.cpp
	private/*.h
	private/Math/*.h
	private/Math/*.cpp
)
file(GLOB
	PUBLIC_FILES
	public/*.h
	public/Math/*.h
)

if (WINDOWS)
	file(GLOB
		PLATFORM_PUBLIC_FILES
		private/Window/Windows/*.h
	)

	file(GLOB
		PLATFORM_PRIVATE_FILES
		private/Window/Windows/*.cpp
	)
endif()

# Add library
include(GenerateExportHeader)
add_library(Core SHARED)
generate_export_header(Core EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME CORE_API)

target_compile_definitions(Core
	PRIVATE
		Core_EXPORTS
		UNICODE)

# Set Sources
target_sources(Core
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_PUBLIC_FILES}
	PRIVATE ${PLATFORM_PRIVATE_FILES}
)

# include dir
target_include_directories(Core 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

# windows-specific
if (WINDOWS)
	target_include_directories(Core 
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Window/Windows
	)
endif()

target_precompile_headers(Core
	PRIVATE
		<unordered_map>
		<vector>
		<string>
		<cassert>
		<format>
		<functional>
)