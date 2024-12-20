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

project(fmod LANGUAGES CXX C)

add_library(fmod STATIC
	include/fmod.h
	include/fmod_common.h
	include/fmod_errors.h
	include/fmod.hpp
	include/fmod_codec.h
	include/fmod_dsp.h
	include/fmod_dsp_effects.h
	include/fmod_output.h
	include/fmod_studio.h
	include/fmod_studio.hpp
	include/fmod_studio_common.h
)

set_target_properties(fmod PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD_REQUIRED ON)

target_include_directories(fmod 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
