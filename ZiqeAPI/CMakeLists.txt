add_custom_target (ZqIncludes SOURCES SystemCalls.h Types.h Process.h Macros.h Memory.h SpinLock.h Mutex.h RWLock.h)

add_subdirectory (${zq_platform})
