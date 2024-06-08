###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(log LANGUAGES C ASM)
###############################################################################
add_library(log STATIC EXCLUDE_FROM_ALL)
target_sources(log PRIVATE ${PROJECT_SOURCE_DIR}/rtt/RTT/SEGGER_RTT.c)
target_sources(log PRIVATE ${PROJECT_SOURCE_DIR}/rtt/RTT/SEGGER_RTT_printf.c)
target_sources(log PRIVATE ${PROJECT_SOURCE_DIR}/rtt/RTT/SEGGER_RTT_ASM_ARMv7M.S)
target_include_directories(log PUBLIC ${PROJECT_SOURCE_DIR}/rtt/RTT)
target_include_directories(log PUBLIC ${PROJECT_SOURCE_DIR}/rtt/Config)
###############################################################################
