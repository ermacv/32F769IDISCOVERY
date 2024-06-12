###############################################################################
# Minimal CMake version that is needed to process current project
cmake_minimum_required(VERSION 3.20)
# Main project name
project(ll_init LANGUAGES C)
###############################################################################
add_library(ll_init STATIC EXCLUDE_FROM_ALL)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/ll_init.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/sdram.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/clock.c)
# target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/qspi.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/ft6x06/ft6x06.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/otm8009a/otm8009a.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/stm32f769i_discovery_qspi.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/stm32f769i_discovery_lcd.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/stm32f769i_discovery.c)
target_sources(ll_init PRIVATE ${PROJECT_SOURCE_DIR}/stm32f769i_discovery_ts.c)
target_link_libraries(ll_init PRIVATE stm32_hal_drivers)
target_link_libraries(ll_init PRIVATE freertos)
target_link_libraries(ll_init PRIVATE syscalls)
target_compile_definitions(ll_init PRIVATE TS_MULTI_TOUCH_SUPPORTED)
###############################################################################
