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

project(lua LANGUAGES CXX C)

file(GLOB
	SRC_FILES
	src/*.c
	src/*.h
	src/*.hpp
)

add_library(lua STATIC)

target_sources(lua
	PRIVATE ${SRC_FILES}
)

set_target_properties(lua PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD_REQUIRED ON)

target_include_directories(lua 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
