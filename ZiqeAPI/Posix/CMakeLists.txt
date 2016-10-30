function(zq_add_driver name)
    add_executable ("${name}" EntryPoint.c Logging.c Memory.c Mutex.c Process.c Socket.c SpinLock.c SystemCalls.c)

    foreach (library ${ARGN})
        target_link_libraries("${name}" ${library})
    endforeach()
endfunction()
