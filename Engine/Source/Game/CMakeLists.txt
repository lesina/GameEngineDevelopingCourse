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
		public/Launcher/Windows/*.h
	)

	file(GLOB
		PLATFORM_SRC_FILES
		private/Launcher/Windows/*.cpp
	)
endif()

# Add executable
if (WINDOWS)
	add_executable(Game WIN32)
else()
	add_executable(Game)
endif()

# Set Sources
target_sources(Game
	PRIVATE ${SRC_FILES}
	PRIVATE ${H_FILES}
	PRIVATE ${PLATFORM_SRC_FILES}
	PRIVATE ${PLATFORM_H_FILES}
)

# include dir
target_include_directories(Game 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_link_libraries(Game PRIVATE
	Core
)

target_precompile_headers(Game
	PRIVATE
		<unordered_map>
		<vector>
)