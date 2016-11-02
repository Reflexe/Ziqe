macro(zq_get_current_linux_version output_name)
    # Run uname -r
    execute_process (COMMAND uname -r OUTPUT_VARIABLE ${output_name} OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro ()

# Linux's version; by default, the current.
# Will be used to find Linux's headers.
#set (zq_linux_version "")
set(zq_linux_version "${CMAKE_SYSTEM_VERSION}" CACHE STRING "Linux version for drivers")

set(zq_linux_here ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")

# Linux X-obj param would get only relative parameter; but,
# we can only get the absolute path to our library; therefore,
# we would insert the relative path of root (/) from this directory.
# (example: ../../../home/user/project/file instead of /home/user/project/file)
file(RELATIVE_PATH zq_here_to_root ${zq_linux_here} /)
set(zq_here_to_root "${zq_here_to_root}" CACHE INTERNAL "A relative path to this directory")

macro(zq_linux_get_library_path current_target output_name zq_current_bin_to_root)
    set (${output_name} "${zq_current_bin_to_root}$<TARGET_LINKER_FILE:${current_target}>")
endmacro ()


set(SOURCES SystemCalls.c Process.c Memory.c
    SpinLock.c Mutex.c EntryPoint.c Socket.c Logging.c )

set(SOURCES ${SOURCES} CACHE INTERNAL "The ZiqeAPI's Linux module's files")

function(zq_add_driver name)
    foreach(file ${SOURCES})
        set(FULL_SOURCES ${FULL_SOURCES} "${zq_linux_here}/${file}")
    endforeach()

    zq_library_to_target(libraries ${ARGN})

    file(RELATIVE_PATH zq_current_binary_to_root "${CMAKE_CURRENT_BINARY_DIR}/Output" /)

    foreach(library ${libraries})
        zq_linux_get_library_path (${library} library_path "${zq_current_binary_to_root}")
        set(ZQ_OBJECTS ${ZQ_OBJECTS} ${library_path})
    endforeach()

    string (REPLACE ";" " " MODULE_SOURCES "${SOURCES}")
    string (REPLACE ";" " " MODULE_OBJECTS "${ZQ_OBJECTS}")

    add_custom_target (${name} ALL
        COMMAND "${zq_linux_here}/CompileLinuxModule.sh" "${MODULE_SOURCES}" "${MODULE_OBJECTS}" "${zq_linux_here}/Makefile"
                "${zq_linux_version}" "${CMAKE_CURRENT_BINARY_DIR}" "${name}"
        SOURCES ${FULL_SOURCES} "${zq_linux_here}/Makefile" "${zq_linux_here}/CompileLinuxModule.sh"
        WORKING_DIRECTORY "${zq_linux_here}"
        COMMENT "Building Linux module: ${name}"
        DEPENDS ${libraries} ${FULL_SOURCES})

     # Make the other target include Linux's (linux/X) headers.
     #target_include_directories (${name} BEFORE PUBLIC "/usr/lib/modules/${zq_linux_version}/build/include/")
endfunction ()
