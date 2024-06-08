###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(freertos LANGUAGES C)
###############################################################################
add_library(freertos STATIC EXCLUDE_FROM_ALL)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/croutine.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/event_groups.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/list.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/queue.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/stream_buffer.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/tasks.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/timers.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/freertos/portable/GCC/ARM_CM7/r0p1/port.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/port/heap_tlsf.c)
target_sources(freertos PRIVATE ${PROJECT_SOURCE_DIR}/port/stack_overflow.c)
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/freertos/include)
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/freertos/portable/GCC/ARM_CM7/r0p1)
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/port)
target_link_libraries(freertos PRIVATE heap)
###############################################################################
