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
add_library(VulkanRHI SHARED)
generate_export_header(VulkanRHI EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME VULKAN_API)

set_target_properties(VulkanRHI PROPERTIES FOLDER "Engine")

target_compile_definitions(VulkanRHI
	PRIVATE
		Vulkan_EXPORTS
		UNICODE)

# Set Sources
target_sources(VulkanRHI
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
)

# include dir
target_include_directories(VulkanRHI
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/Texture
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../RHI/public
	PRIVATE ${CMAKE_LIBRARY_PATH}/glslang
	PRIVATE ${CMAKE_LIBRARY_PATH}/volk
)

target_link_libraries(VulkanRHI PRIVATE
	Math
	RenderCore
	Core
	volk
	dxcompiler.lib
)

target_precompile_headers(VulkanRHI
	PRIVATE
		<array>
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