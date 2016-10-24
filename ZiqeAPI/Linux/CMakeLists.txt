macro(zq_get_current_linux_version output_name)
    # Run uname -r
    execute_process (COMMAND uname -r OUTPUT_VARIABLE ${output_name} OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro ()

# Linux's version; by default, the current.
# Will be used to find Linux's headers.
#set (zq_linux_version "")
zq_get_current_linux_version (zq_linux_version)

# Linux X-obj param would get only relative parameter; but,
# we can only get the absolute path to our library; therefore,
# we would insert the relative path of root (/) from this directory.
# (example: ../../../home/user/project/file instead of /home/user/project)
file(RELATIVE_PATH zq_here_to_root ${CMAKE_CURRENT_SOURCE_DIR} /)

macro(zq_get_lib_linker_flags output_name lib_name)
    set (current_target ${zq_target_prefix}${lib_name})
    set (${output_name} "${zq_here_to_root}$<TARGET_LINKER_FILE:${current_target}>")

    # Make the other target include Linux's (linux/X) headers.
    target_include_directories (${current_target} BEFORE PUBLIC "/usr/lib/modules/${zq_linux_version}/build/include/")
endmacro ()

macro(zq_linux_link_static_library lib_name)
    zq_get_lib_linker_flags(lib_linker_flags ${lib_name})
    set (ZQ_OBJECTS "${ZQ_OBJECTS} ${lib_linker_flags}")
endmacro ()

set(SOURCES Linux.c SystemCalls.c Process.c Memory.c SpinLock.c Mutex.c EntryPoint.c Socket.c)

zq_linux_link_static_library (Main)
zq_linux_link_static_library (ZiqeProtocol)
zq_linux_link_static_library (Core)
zq_linux_link_static_library (Network)
zq_linux_link_static_library (Base)

string (REPLACE ";" " " MODULE_SOURCES "${SOURCES}")
add_custom_target (LinuxZiqeAPI ALL
                   "${CMAKE_CURRENT_SOURCE_DIR}/CompileLinuxModule.sh" "${MODULE_SOURCES}" "${ZQ_OBJECTS}"
                                                                       "${CMAKE_CURRENT_SOURCE_DIR}/Makefile"
                                                                       "${zq_linux_version}"
                   SOURCES ${SOURCES} Makefile
                   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                   COMMENT "Building Linux module")
