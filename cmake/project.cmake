###############################################################################
include(${CMAKE_CURRENT_LIST_DIR}/cpu/stm32f769.cmake)
# Include common warning
include(${CMAKE_CURRENT_LIST_DIR}/build_common/warnings.cmake)
# Include common build options that should ne used by all project modules and libraries
include(${CMAKE_CURRENT_LIST_DIR}/build_common/compiler.cmake)
# Include common linker options
include(${CMAKE_CURRENT_LIST_DIR}/build_common/linker.cmake)
###############################################################################
add_compile_options(-Os) # TODO: Temporary
###############################################################################

