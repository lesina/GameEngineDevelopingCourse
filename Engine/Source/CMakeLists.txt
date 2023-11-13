cmake_minimum_required(VERSION 3.19)

include_directories(${PROJECT_BINARY_DIR}/Engine/Source)

add_subdirectory(Core)
add_subdirectory(Game)
add_subdirectory(RenderEngine)
add_subdirectory(RHI)
