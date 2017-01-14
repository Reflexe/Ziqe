set (SOURCES Memory.cpp OS/Memory.h Device.cpp OS/Device.hpp
             )

add_custom_target (_wdklib SOURCES ${SOURCES})
