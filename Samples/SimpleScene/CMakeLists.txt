cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

SET(PROJECT_NAME "" CACHE STRING "project_name")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_compile_definitions(
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:RELEASE>
)

project(${PROJECT_NAME} LANGUAGES CXX)

if (WINDOWS)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Win64/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Win64/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Win64/bin)
endif()

include_directories(${PROJECT_BINARY_DIR}/Engine/Source)
include_directories(${PROJECT_BINARY_DIR}/Engine/Editor)
include_directories(${PROJECT_BINARY_DIR}/Source)

add_subdirectory(${CMAKE_SOURCE_DIR}/../../Engine/ Engine)
add_subdirectory(Source)
