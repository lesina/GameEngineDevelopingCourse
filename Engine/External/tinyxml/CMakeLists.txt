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

project(tinyxml LANGUAGES CXX C)

add_library(tinyxml STATIC
	src/tinyxml2.cpp
	include/tinyxml2.h
)

set_target_properties(tinyxml PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD_REQUIRED ON)

target_include_directories(tinyxml 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
