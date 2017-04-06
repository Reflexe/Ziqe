set (SOURCES OS/Memory.cpp OS/Memory.hpp OS/Device.cpp OS/Device.hpp
             ZqAPI/Memory.h ZqAPI/EntryPoints.c)

zq_add_library (NAME _wdklib SOURCES ${SOURCES} INCLUDES /usr/include/wine/windows/)
