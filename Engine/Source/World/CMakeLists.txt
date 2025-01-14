# get all files
file(GLOB
	PRIVATE_FILES
	private/*.h
	private/*.cpp
	private/Parser/*.h
	private/Parser/*.cpp
	public/Parser/*.h
	public/Parser/*.cpp
)
file(GLOB
	PUBLIC_FILES
	public/*.h
)

# Add library
include(GenerateExportHeader)
add_library(World SHARED)
generate_export_header(World EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME WORLD_API)

set_target_properties(World PROPERTIES FOLDER "Engine")

#setting tinyxml libs
if (WINDOWS)
	find_library(TINYXML_DEBUG NAMES tinyxml
		PATHS ${CMAKE_LIBRARY_PATH}/tinyxml/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(TINYXML_RELEASE NAMES tinyxml 
		PATHS ${CMAKE_LIBRARY_PATH}/tinyxml/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(TINYXML_LIBRARIES debug ${TINYXML_DEBUG} optimized ${TINYXML_RELEASE})
endif()

target_compile_definitions(World
	PRIVATE
		World_EXPORTS
		UNICODE)

# Set Sources
target_sources(World
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(World 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/tinyxml/include
)

target_link_libraries(World PRIVATE
	Core
	Math
	EntitySystem
	RenderCore
	AudioSystem
	
	${TINYXML_LIBRARIES}
)

target_precompile_headers(World
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