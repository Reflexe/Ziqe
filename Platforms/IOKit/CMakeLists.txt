zq_add_library(NAME IOKitLib
    SOURCES OS/Memory.cpp OS/Memory.hpp ZqAPI/EntryPoints.cpp
    INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/ZqAPI/bsd/")
