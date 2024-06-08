###############################################################################
# Setup Compile options
add_compile_options(-mcpu=cortex-m7)
add_compile_options(-mfpu=fpv5-d16)
add_compile_options(-mfloat-abi=hard)
###############################################################################
# Setup Linker options
add_link_options(-mcpu=cortex-m7)
add_link_options(-mfpu=fpv5-d16)
add_link_options(-mfloat-abi=hard)
###############################################################################
