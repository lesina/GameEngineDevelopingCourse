# get all files
file(GLOB
	SRC_FILES
	private/*.cpp
)
file(GLOB
	H_FILES
	public/*.h
)

if (WINDOWS)
	file(GLOB
		PLATFORM_H_FILES
		public/WindowHandle/Windows/*.h
	)

	file(GLOB
		PLATFORM_SRC_FILES
		private/WindowHandle/Windows/*.cpp
	)
endif()

# Add library
include(GenerateExportHeader)
add_library(Core SHARED)
generate_export_header(Core EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME CORE_API)

target_compile_definitions(Core PRIVATE Core_EXPORTS)

# Set Sources
target_sources(Core
	PRIVATE ${SRC_FILES}
	PRIVATE ${H_FILES}
	PRIVATE ${PLATFORM_H_FILES}
	PRIVATE ${PLATFORM_SRC_FILES}
)

# include dir
target_include_directories(Core 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
)

# windows-specific
if (WINDOWS)
	target_include_directories(Core 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/WindowHandle/Windows
	)
endif()

target_precompile_headers(Core
	PRIVATE
		<unordered_map>
		<vector>
)