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
add_library(Direct3D12 SHARED)
generate_export_header(Direct3D12 EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME DIRECT3D_API)

target_compile_definitions(Direct3D12 PRIVATE Direct3D12_EXPORTS)

# Set Sources
target_sources(Direct3D12
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(Direct3D12 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../RHI/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../External/Direct3DHeaders/include/directx
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../External/Direct3DTexture/DDSTextureLoader
)

target_precompile_headers(Direct3D12
	PRIVATE
		<unordered_map>
		<vector>
		<array>
		<string>
		<cassert>
		<format>
)