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
add_library(AudioSystem SHARED)
generate_export_header(AudioSystem EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME AUDIO_SYSTEM_API)

set_target_properties(AudioSystem PROPERTIES FOLDER "Engine")

#setting libs
if (WINDOWS)
	find_library(FMOD_DEBUG NAMES fmod
		PATHS ${CMAKE_LIBRARY_PATH}/fmod/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(FMOD_RELEASE NAMES fmod
		PATHS ${CMAKE_LIBRARY_PATH}/fmod/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(FMOD_LIBRARIES debug ${FMOD_DEBUG} optimized ${FMOD_RELEASE})
endif()

target_compile_definitions(AudioSystem
	PRIVATE
		AudioSystem_EXPORTS
		UNICODE)

# Set Sources
target_sources(AudioSystem
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(AudioSystem 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/fmod/include
)

target_link_libraries(AudioSystem PRIVATE
	Core
	Math

	${FMOD_LIBRARIES}
)

target_precompile_headers(AudioSystem
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
		<bitset>
		<queue>
		<iostream>
)