###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(syscalls LANGUAGES C)
###############################################################################
add_library(syscalls INTERFACE)
target_sources(syscalls INTERFACE ${PROJECT_SOURCE_DIR}/syscalls.c)
target_sources(syscalls INTERFACE ${PROJECT_SOURCE_DIR}/ll_init.c)
target_link_libraries(syscalls INTERFACE heap)
target_link_libraries(syscalls INTERFACE log)
###############################################################################
