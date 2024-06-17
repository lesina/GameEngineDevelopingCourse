cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:RELEASE>
)

project(flecsLua LANGUAGES CXX C)

file(GLOB
	SRC_FILES
	src/*.c
	src/*.h
)

add_library(flecsLua STATIC)

target_sources(flecsLua
	PRIVATE ${SRC_FILES}
)

set_target_properties(flecsLua PROPERTIES
            LINKER_LANGUAGE CXX
            CXX_STANDARD_REQUIRED ON)

#setting libs
if (WINDOWS)
	find_library(FLECS_DEBUG NAMES flecs
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../flecs/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(FLECS_RELEASE NAMES flecs 
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../flecs/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(FLECS_LIBRARIES debug ${FLECS_DEBUG} optimized ${FLECS_RELEASE})
	
	find_library(LUA_DEBUG NAMES lua
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../lua/lib/Windows/Debug
		NO_DEFAULT_PATH)
	find_library(LUA_RELEASE NAMES lua 
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../lua/lib/Windows/Release
		NO_DEFAULT_PATH)
	set(LUA_LIBRARIES debug ${LUA_DEBUG} optimized ${LUA_RELEASE})
endif()

target_link_libraries(flecsLua PRIVATE
	${FLECS_LIBRARIES}
	${LUA_LIBRARIES}
)

target_include_directories(flecsLua PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/../flecs/include
	${CMAKE_CURRENT_SOURCE_DIR}/../lua/src
)

target_compile_definitions(flecsLua
	PRIVATE
		flecs_lua_STATIC
)

