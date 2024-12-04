# get all files
file(GLOB
	PRIVATE_FILES
	private/*.cpp
	private/*.h
	private/Texture/*.cpp
)

file(GLOB
	PUBLIC_FILES
	public/*.h
)


# Add library
include(GenerateExportHeader)
add_library(Direct3D12RHI SHARED)
generate_export_header(Direct3D12RHI EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME DIRECT3D_API)

set_target_properties(Direct3D12RHI PROPERTIES FOLDER "Engine")

target_compile_definitions(Direct3D12RHI
	PRIVATE
		Direct3D12_EXPORTS
		UNICODE)

# Set Sources
target_sources(Direct3D12RHI
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(Direct3D12RHI 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Texture
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../RHI/public
	PRIVATE ${CMAKE_LIBRARY_PATH}/Direct3DHeaders/include/directx
)

target_link_libraries(Direct3D12RHI PRIVATE
	Math
	RenderCore
	Core
	d3dcompiler.lib
	D3D12.lib
	dxgi.lib
)

target_precompile_headers(Direct3D12RHI
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
)