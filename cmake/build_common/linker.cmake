###############################################################################
# Setup common Linker options
###############################################################################
add_link_options(-ffunction-sections)
add_link_options(-fdata-sections)
add_link_options(-Wl,--gc-sections)
add_link_options(-Wl,--print-memory-usage)
add_link_options(--specs=nano.specs)
###############################################################################
# Wrap functions
add_link_options(-Wl,-wrap,puts)
add_link_options(-Wl,-wrap,printf)
add_link_options(-Wl,-wrap,malloc)
add_link_options(-Wl,-wrap,calloc)
add_link_options(-Wl,-wrap,realloc)
add_link_options(-Wl,-wrap,free)
###############################################################################
