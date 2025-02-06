# get all files
file(GLOB
	PRIVATE_FILES
	private/*.cpp
	private/*.h
	private/EditorECS/*.cpp
	private/EditorECS/*.h
	private/LevelEditor/*.cpp
	private/LevelEditor/*.h
	private/LevelEditor/ECS/*.cpp
	private/LevelEditor/ECS/*.h
)
file(GLOB
	PUBLIC_FILES
	public/*.h
	public/EditorECS/*.h
	public/LevelEditor/*.h
	public/LevelEditor/ECS/*.h
)

# Add executable
add_executable(Editor WIN32)

set_target_properties(Editor PROPERTIES FOLDER "Editor")

target_compile_definitions(Editor
	PRIVATE
		UNICODE)

if (WINDOWS)
	find_library(IMGUI_DEBUG NAMES imgui
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(IMGUI_RELEASE NAMES imgui 
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(IMGUI_LIBRARIES debug ${IMGUI_DEBUG} optimized ${IMGUI_RELEASE})
	
	#setting tinyxml libs
	find_library(TINYXML_DEBUG NAMES tinyxml
		PATHS ${CMAKE_LIBRARY_PATH}/tinyxml/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(TINYXML_RELEASE NAMES tinyxml 
		PATHS ${CMAKE_LIBRARY_PATH}/tinyxml/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(TINYXML_LIBRARIES debug ${TINYXML_DEBUG} optimized ${TINYXML_RELEASE})
endif()

# Set Sources
target_sources(Editor
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(Editor 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/imgui/include
	PRIVATE ${CMAKE_LIBRARY_PATH}/tinyxml/include
)

target_link_libraries(Editor PRIVATE
	Core
	EntitySystem
	GUI
	Math
	RenderCore
	RenderEngine
	ScriptSystem
	World
	
	${IMGUI_LIBRARIES}
	${TINYXML_LIBRARIES}
)

target_precompile_headers(Editor
	PRIVATE
		<algorithm>
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
		<variant>
		<stack>
		<semaphore>
)