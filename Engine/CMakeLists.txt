set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/External)

add_subdirectory(Source)

if (WINDOWS)
	add_subdirectory(Editor)
endif ()