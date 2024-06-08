###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(heap LANGUAGES C)
###############################################################################
add_library(heap STATIC EXCLUDE_FROM_ALL)
target_sources(heap PRIVATE ${PROJECT_SOURCE_DIR}/heap.c)
target_sources(heap PRIVATE ${PROJECT_SOURCE_DIR}/tlsf/src/tlsf.c)
target_include_directories(heap PRIVATE ${PROJECT_SOURCE_DIR}/tlsf/src)
target_include_directories(heap PRIVATE ${PROJECT_SOURCE_DIR}/port)
target_include_directories(heap PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_definitions(heap PRIVATE TLSF_STATISTIC)
target_link_libraries(heap PRIVATE freertos)
###############################################################################
