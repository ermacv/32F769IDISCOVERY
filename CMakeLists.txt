###############################################################################
cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
###############################################################################
if(NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain/gcc-arm-none-eabi.cmake)
endif()
###############################################################################
project(test_project)
###############################################################################
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
###############################################################################
add_subdirectory(${PROJECT_SOURCE_DIR}/components/syscalls)
add_subdirectory(${PROJECT_SOURCE_DIR}/components/drivers)
add_subdirectory(${PROJECT_SOURCE_DIR}/components/startup)
add_subdirectory(${PROJECT_SOURCE_DIR}/components/main)
###############################################################################
