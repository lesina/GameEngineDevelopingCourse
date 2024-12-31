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
		public/Windows/*.h
	)

	file(GLOB
		PLATFORM_PRIVATE_FILES
		private/Windows/*.cpp
		private/Windows/*.h
	)
endif()

# Add library
include(GenerateExportHeader)
add_library(RHI SHARED)
generate_export_header(RHI EXPORT_FILE_NAME export.h EXPORT_MACRO_NAME RHI_API)

set_target_properties(RHI PROPERTIES FOLDER "Engine")

target_compile_definitions(RHI
	PRIVATE
		RHI_EXPORTS
		UNICODE)

# Set Sources
target_sources(RHI
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_PUBLIC_FILES}
	PRIVATE ${PLATFORM_PRIVATE_FILES}
)

# include dir
target_include_directories(RHI 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)

if (WINDOWS)
	target_include_directories(RHI 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public/Windows
	)

	target_link_libraries(RHI PRIVATE
		Direct3D12RHI
		VulkanRHI
	)
endif()

target_link_libraries(RHI PRIVATE
	Math
	RenderCore
	Core
)

target_precompile_headers(RHI
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