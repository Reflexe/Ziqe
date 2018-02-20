** This page is obselete **

## ZiqeAPI
ZiqeAPI a cross-platform C API for Drivers and Applications.

## Supported Platforms:
    * Linux (Drivers)
    * Posix (Applications)
    * Windows NT (Vista+)
    * IOKit: iOS, macOS.

## ZiqeAPI's model:
To support a platform, there should be a directory with its name
that contains a CMakeLists.txt and implements the following functions:

    - zq_add_driver(name driver_sources): Add a driver for compiling.
        - @param name:            The driver's name.
        - @param driver_sources:  The sources to compile.

      The output directory should be CMAKE_CURRENT_BUILD_DIRECTORY.

On the begining, the driver should call `void CppCoreInit (void);` and `void CppCoreExit (void);` on
exit. These functions should not block.


# The build system
What I want?
    copts being resolved in runtime by a provider and deps.

The problem:
    No way to change copts in runtime.

What I want:
    Platforms being detected and loaded dynamicly in runtime.
The problem:
    No way to load() dynamicly.


What I want:
    special cc_library API like zq_config and zq_driver
No problem.
