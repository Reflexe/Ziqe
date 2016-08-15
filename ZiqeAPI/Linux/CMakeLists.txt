execute_process (COMMAND uname -r OUTPUT_VARIABLE zq_linux_version OUTPUT_STRIP_TRAILING_WHITESPACE)

set(SOURCES Linux.c Linux.h SystemCalls.c Process.c Memory.c SpinLock.c Mutex.c)
#add_custom_target (LinuxZiqeAPI ALL COMMAND "make" -f "${CMAKE_CURRENT_SOURCE_DIR}/Makefile" SOURCES ${SOURCES} Makefile COMMENT "Building Linux module")


add_library (LinuxZiqeAPI ${SOURCES})
target_include_directories (LinuxZiqeAPI BEFORE PRIVATE "/usr/lib/modules/${zq_linux_version}/build/include/"
    "/usr/lib/modules/${zq_linux_version}/build/arch/x86/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../")
