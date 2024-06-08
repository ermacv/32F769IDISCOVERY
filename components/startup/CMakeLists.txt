###############################################################################
cmake_minimum_required(VERSION 3.20)
###############################################################################
project(test_project LANGUAGES C ASM)
###############################################################################
add_library(stm32_startup INTERFACE)
target_sources(stm32_startup INTERFACE ${PROJECT_SOURCE_DIR}/startup_stm32f769xx.s)
target_sources(stm32_startup INTERFACE ${PROJECT_SOURCE_DIR}/system_stm32f7xx.c)
target_link_options(stm32_startup INTERFACE -T${PROJECT_SOURCE_DIR}/STM32F769NIHX_FLASH.ld)
set_target_properties(stm32_startup PROPERTIES LINK_DEPENDS ${PROJECT_SOURCE_DIR}/STM32F769NIHX_FLASH.ld)
target_link_libraries(stm32_startup INTERFACE stm32_hal_drivers)
###############################################################################