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
		public/Platforms/Windows/*.h
	)

	file(GLOB
		PLATFORM_PRIVATE_FILES
		private/Platforms/Windows/*.cpp
		private/Platforms/Windows/*.h
	)
	
	file(GLOB
		RENDER_BACKENDS_PUBLIC_FILES
		public/RenderBackends/D3D12/*.h
	)

	file(GLOB
		RENDER_BACKENDS_PRIVATE_FILES
		private/RenderBackends/D3D12/*.cpp
		private/RenderBackends/D3D12/*.h
	)
	
	find_library(IMGUI_DEBUG NAMES imgui
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(IMGUI_RELEASE NAMES imgui 
		PATHS ${CMAKE_LIBRARY_PATH}/imgui/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(IMGUI_LIBRARIES debug ${IMGUI_DEBUG} optimized ${IMGUI_RELEASE})
endif()

# Add library
include(GenerateExportHeader)
add_library(GUI SHARED)
generate_export_header(GUI EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME GUI_API)

set_target_properties(GUI PROPERTIES FOLDER "Engine")

target_compile_definitions(GUI
	PRIVATE
		GUI_EXPORTS
		UNICODE)

# Set Sources
target_sources(GUI
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_PUBLIC_FILES}
	PRIVATE ${PLATFORM_PRIVATE_FILES}
	PRIVATE ${RENDER_BACKENDS_PUBLIC_FILES}
	PRIVATE ${RENDER_BACKENDS_PRIVATE_FILES}
)

# include dir
target_include_directories(GUI 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_LIBRARY_PATH}/imgui/include
)

target_link_libraries(GUI PRIVATE
	Core
	Math
	RenderCore
	RHI
	${IMGUI_LIBRARIES}
)

if (WINDOWS)
	target_include_directories(GUI 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/Platforms/Windows
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/RenderBackends/D3D12
		
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Platforms/Windows
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/RenderBackends/D3D12
		
		PRIVATE ${CMAKE_LIBRARY_PATH}/Direct3DHeaders/include/directx
	)

	target_link_libraries(GUI PRIVATE
		Direct3D12RHI
		
		d3dcompiler.lib
		D3D12.lib
		dxgi.lib
	)
endif()

target_precompile_headers(GUI
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
		<filesystem>
		<variant>
		<stack>
		<queue>
)