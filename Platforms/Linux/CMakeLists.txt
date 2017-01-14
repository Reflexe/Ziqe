# Linux's version; by default, the current.
# Will be used to find Linux's headers.
#set (zq_linux_version "")
unset(zq_linux_version CACHE)
set(zq_linux_version "${CMAKE_SYSTEM_VERSION}" CACHE STRING "Linux version for drivers")

# Include the linux C API and module
add_subdirectory (ZqAPI)
# Add the C++ library.
add_subdirectory (OS)

# @brief The main add driver function for linux.
# @param NAME       The driver's name.
# @param LIBRARIES  Libraries to link this module to.
function(zq_platform_add_driver)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME)
    set(_MULTI_VALUE_ARGS LIBRARIES)

    cmake_parse_arguments(DRIVER "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    if (NOT DRIVER_NAME)
        message(FATAL_ERROR "DRIVER_NAME is not set.")
    endif()

    #if (NOT DRIVER_LIBRARIES)
    #    message(FATAL_ERROR "DRIVER_LIBRARIES is not set.")
    #endif()

    # Compile the module.
    zq_add_linux_module (NAME "${DRIVER_NAME}"
                         SOURCES ${zq_linux_zqapi_sources}
                         LIBRARIES ${DRIVER_LIBRARIES}
                         LINUX_VERSION ${zq_linux_version}
                         SOURCES_PATH "${zq_linux_here}"
                         INCLUDE_PATHS "${zq_root}" "${zq_linux_here}/../")
endfunction()
