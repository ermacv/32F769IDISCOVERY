###############################################################################
# Common compiler options
add_compile_options(-fdata-sections)
add_compile_options(-ffunction-sections)
# add_compile_options(-fno-jump-tables)
# add_compile_options(-fno-common)
# add_compile_options(-fms-extensions)
# add_compile_options(-fmessage-length=0)
# add_compile_options(-fstrict-volatile-bitfields)
# add_compile_options(-fshort-enums)
# add_compile_options(-fstack-usage)
# add_compile_options(-save-temps=obj)
###############################################################################
# C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
###############################################################################
# CXX standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
###############################################################################
