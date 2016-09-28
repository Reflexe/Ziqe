add_custom_target (ZqIncludes SOURCES SystemCalls.h Types.h Process.h Macros.h Memory.h SpinLock.h Mutex.h RWLock.h Socket.h)

macro(zq_define_per_platform_file_generator Name)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${zq_platform}/${Name}.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/${Name}.h.generated")
endmacro ()

zq_define_per_platform_file_generator (ProcessConfig)
zq_define_per_platform_file_generator (MemoryConfig)
zq_define_per_platform_file_generator (MemoryInline)
zq_define_per_platform_file_generator (SocketConfig)

add_subdirectory (${zq_platform})
