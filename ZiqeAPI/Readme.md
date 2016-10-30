## ZiqeAPI
ZiqeAPI a cross-platform C API for Drivers and Applications.

## Supported Platforms:
    * Linux (Drivers)
    * Posix (Applications)
    * (TODO) Windows NT (Vista+)

## ZiqeAPI's model:
To support a platform, there should be a directory with its name
that contains a CMakeLists.txt and implements the following functions:
    * zq_add_driver(name driver_sources): Add a driver for compiling.
        @param name:            The driver's name.
        @param driver_sources:  The sources to compile.

      The output directory should be CMAKE_CURRENT_BUILD_DIRECTORY.

On the begining, the driver should call `void ZqAPIInit (void);` and `void ZqAPIExit (void);` on
exit. These functions should not block.
