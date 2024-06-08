###############################################################################
cmake_minimum_required(VERSION 3.20)
###############################################################################
project(main C ASM)
###############################################################################
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE stm32_startup)
target_link_libraries(${PROJECT_NAME} PRIVATE syscalls)
target_link_libraries(${PROJECT_NAME} PRIVATE freertos)
target_link_options(${PROJECT_NAME} PRIVATE -Wl,-Map=$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.map)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
###############################################################################
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.map ${PROJECT_NAME}.map
  COMMAND ${CMAKE_OBJCOPY} ARGS -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} ARGS -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
###############################################################################
add_custom_target(flash
    COMMAND JLinkExe -CommandFile ${PROJECT_SOURCE_DIR}/jlink.jlink
    DEPENDS ${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_SOURCE_DIR}/jlink.jlink
)
###############################################################################
