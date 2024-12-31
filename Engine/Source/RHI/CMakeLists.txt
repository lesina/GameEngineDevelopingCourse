cmake_minimum_required(VERSION 3.19)

add_subdirectory(RHI)

if (WINDOWS)
	add_subdirectory(Direct3D12RHI)
	add_subdirectory(VulkanRHI)
endif()