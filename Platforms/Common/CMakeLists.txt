macro (zq_common_add_driver)
    zq_platform_add_driver (${ARGN})
endmacro()

#include_directories ("${zq_root}/Platforms/Common/")

add_subdirectory (OS)
