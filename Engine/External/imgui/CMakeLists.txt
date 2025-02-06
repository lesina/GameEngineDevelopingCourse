cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:RELEASE>
)

project(imgui LANGUAGES CXX C)

file(GLOB
	PRIVATE_FILES
	src/*.h
	src/*.cpp
)

file(GLOB
	PUBLIC_FILES
	include/*.h
)

if (WINDOWS)
	file(GLOB
		PLATFORM_FILES
		src/backends/imgui_impl_win32.cpp
		include/backends/imgui_impl_win32.h
		src/backends/imgui_impl_dx12.cpp
		include/backends/imgui_impl_dx12.h
	)
endif()

add_library(imgui STATIC)

set_target_properties(imgui PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD_REQUIRED ON)

target_sources(imgui
	PRIVATE ${PRIVATE_FILES}
	PRIVATE ${PUBLIC_FILES}
	PRIVATE ${PLATFORM_FILES}
)

target_include_directories(imgui 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WINDOWS)
	target_link_libraries(imgui PRIVATE
		d3dcompiler.lib
		D3D12.lib
		dxgi.lib
	)
endif()