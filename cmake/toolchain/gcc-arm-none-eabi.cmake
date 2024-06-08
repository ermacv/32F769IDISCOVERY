###############################################################################
#CPU setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
# There is a CMAKE_CROSSCOMPILING variable. Checking this variable CMake can
# understand what compiler is used and what is a target platform
# You don't need to set this variable you set CMAKE_SYSTEM_NAME and CMAKE_SYSTEM_PROCESSOR
# that are differs from a current host system (it sets automatically)
###############################################################################
# Compiler version preconditions
set(COMPILER_VERSION_REQ    "10.2.0" CACHE INTERNAL "ARM GCC Compiler required version")
set(TARGET_TRIPLET          "arm-none-eabi" CACHE INTERNAL "ARM GCC Toolchain target triplet")
# TODO: Use configurable path
set(GCC_COMPILER_PATH       "$ENV{ARM_NONE_EABI_TOOLCHAIN}" CACHE INTERNAL "GCC Compiler Path")
set(COMPILER_EXE_EXTENSION  "")
###############################################################################
# Compiler pathes information
set(COMPILER_BIN ${GCC_COMPILER_PATH}/bin)

# Get compiler version by executing a command
execute_process(
	COMMAND ${COMPILER_BIN}/${TARGET_TRIPLET}-gcc${COMPILER_EXE_EXTENSION} -dumpversion 
	OUTPUT_VARIABLE COMPILER_VERSION 
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Check that current compiler version is the same that was expected
if(NOT(COMPILER_VERSION STREQUAL COMPILER_VERSION_REQ))
	# Newer versions of ARM GCC compiler (at least 2019 year realeses) have another pathes for system includes
	# message(WARNING "GCC version ${COMPILER_VERSION} isn't supported by the project (${COMPILER_VERSION_REQ})\nYou should provide a manual support\n")
endif()
###############################################################################
# Set compiler binutils
set(CMAKE_C_COMPILER "${COMPILER_BIN}/${TARGET_TRIPLET}-gcc${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER "${COMPILER_BIN}/${TARGET_TRIPLET}-gcc${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER "${COMPILER_BIN}/${TARGET_TRIPLET}-g++${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_OBJCOPY "${COMPILER_BIN}/${TARGET_TRIPLET}-objcopy${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_SIZE "${COMPILER_BIN}/${TARGET_TRIPLET}-size${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_OBJDUMP "${COMPILER_BIN}/${TARGET_TRIPLET}-objdump${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_LINKER "${COMPILER_BIN}/${TARGET_TRIPLET}-ld${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_RANLIB "${COMPILER_BIN}/${TARGET_TRIPLET}-ranlib${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_AR "${COMPILER_BIN}/${TARGET_TRIPLET}-ar${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_NM "${COMPILER_BIN}/${TARGET_TRIPLET}-nm${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
set(CMAKE_STRIP "${COMPILER_BIN}/${TARGET_TRIPLET}-strip${COMPILER_EXE_EXTENSION}" CACHE INTERNAL "")
###############################################################################
# CMake compiles a lib before building a project
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
###############################################################################