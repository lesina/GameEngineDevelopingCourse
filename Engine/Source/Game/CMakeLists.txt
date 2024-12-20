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

if (WINDOWS)
	file(GLOB
		PLATFORM_PUBLIC_FILES
		public/Launcher/Windows/*.h
	)

	file(GLOB
		PLATFORM_PRIVATE_FILES
		private/Launcher/Windows/*.cpp
	)
endif()

# Add executable
if (WINDOWS)
	add_executable(Game WIN32)
else()
	add_executable(Game)
endif()

set_target_properties(Game PROPERTIES FOLDER "Engine")

target_compile_definitions(Game
	PRIVATE
		UNICODE)

# Set Sources
target_sources(Game
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_PRIVATE_FILES}
	PRIVATE ${PLATFORM_PUBLIC_FILES}
)

# include dir
target_include_directories(Game 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(Game PRIVATE
	AudioEngine
	Core
	EntitySystem
	Math
	RenderCore
	RenderEngine
	ScriptSystem
)

target_precompile_headers(Game
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