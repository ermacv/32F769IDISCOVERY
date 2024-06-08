###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(stm32_hal_drivers LANGUAGES C ASM)
###############################################################################
add_library(stm32_hal_drivers STATIC EXCLUDE_FROM_ALL)
# target_sources(stm32_hal_drivers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cec.c)
target_sources(stm32_hal_drivers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c)
target_include_directories(stm32_hal_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/STM32F7xx_HAL_Driver/Inc)
target_include_directories(stm32_hal_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include)
target_include_directories(stm32_hal_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include)
target_include_directories(stm32_hal_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32F7xx/Include)
target_include_directories(stm32_hal_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(stm32_hal_drivers PUBLIC STM32F769xx)
###############################################################################
