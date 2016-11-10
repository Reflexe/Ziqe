include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (Protocol)
add_subdirectory (Common)
add_subdirectory (Host)
add_subdirectory (Client)

#zq_add_depend_modules(Core Protocol)
