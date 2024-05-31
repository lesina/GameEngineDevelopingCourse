# get all files
file(GLOB
	PRIVATE_FILES
	private/*.h
	private/*.cpp
	private/Window/*.h
	private/Window/*.cpp
	private/Debug/*.h
	private/Debug/*.cpp
	private/Input/*.h
	private/Input/*.cpp
)
file(GLOB
	PUBLIC_FILES
	public/*.h
	public/Window/*.h
	public/Debug/*.h
	public/Input/*.h
)

if (WINDOWS)
	file(GLOB
		PLATFORM_PRIVATE_FILES
		private/Window/Windows/*.cpp
		private/Window/Windows/*.h
		private/Debug/Windows/*.h
		private/Debug/Windows/*.cpp
		private/Input/Windows/*.h
		private/Input/Windows/*.cpp
	)
endif()

# Add library
include(GenerateExportHeader)
add_library(Core SHARED)
generate_export_header(Core EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME CORE_API)

#setting inih libs
if (WINDOWS)
	find_library(INIH_DEBUG NAMES inih
		PATHS ${CMAKE_LIBRARY_PATH}/inih/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(INIH_RELEASE NAMES inih 
		PATHS ${CMAKE_LIBRARY_PATH}/inih/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(INIH_LIBRARIES debug ${INIH_DEBUG} optimized ${INIH_RELEASE})
endif()

target_compile_definitions(Core
	PRIVATE
		Core_EXPORTS
		UNICODE)

# Set Sources
target_sources(Core
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_PRIVATE_FILES}
)

# include dir
target_include_directories(Core 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/inih/include
)

# windows-specific
if (WINDOWS)
	target_include_directories(Core 
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Window/Windows
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Debug/Windows
	)
endif()

target_link_libraries(Core PRIVATE
	Math
	${INIH_LIBRARIES}
)

target_precompile_headers(Core
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
)