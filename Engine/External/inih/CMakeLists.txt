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

project(inih LANGUAGES CXX C)

add_library(inih STATIC
	src/ini.c
	src/INIReader.cpp
	include/ini.h
	include/INIReader.h
)

set_target_properties(inih PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON)