add_library(Inih STATIC cpp/INIReader.cpp ini.c)

target_include_directories(Inih 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)