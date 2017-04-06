#add_custom_target (ZqIncludes SOURCES SystemCalls.h Types.h Process.h Macros.h
#                   Memory.h SpinLock.h Mutex.h RWLock.h Socket.h Logging.h
#                   EntryPoints.h)

#macro(zq_define_per_platform_file_generator Name)
#    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${zq_platform}/${Name}.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/${Name}.h.generated")
#endmacro ()

#zq_define_per_platform_file_generator (ProcessConfig)
#zq_define_per_platform_file_generator (MemoryConfig)
#zq_define_per_platform_file_generator (MemoryInline)
#zq_define_per_platform_file_generator (SocketConfig)
#zq_define_per_platform_file_generator (Errors)

#add_subdirectory (${zq_platform})

# Linux Ziqe
add_subdirectory (Linux)

# IOKit Ziqe
#add_subdirectory (IOKit)

# WDK Ziqe
#add_subdirectory (WDK)

# Common files and functions (both cpp and cmake) for all platforms
# Any platform can override these by simply having a file in its includes folders.
# e.g. To override IDriver, simply create the file IDriver.h in your platform's
# OS folder.
add_subdirectory (Common)
