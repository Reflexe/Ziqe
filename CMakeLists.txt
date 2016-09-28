project(Ziqe)

cmake_minimum_required(VERSION 2.8)

# Set Ziqe variables.
set(zq_root ${CMAKE_CURRENT_SOURCE_DIR})
set(zq_library_type STATIC)
set(zq_cpp_ext cpp)
set(zq_target_prefix zq_)
set(zq_h_ext hpp)

set(zq_debug 1)
set(zq_test_build 0)
set(zq_platform Linux)

include_directories (${zq_root})
enable_testing()

if(zq_debug EQUAL "1")
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

function(__zq_add_module caller_name target_path)
    get_filename_component(target_name ${target_path} NAME)

    if(NOT IS_DIRECTORY "${target_path}/")
       message(FATAL_ERROR "${target_path} is not exist!")
    endif(NOT IS_DIRECTORY "${target_path}/")

    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "${target_name} is not a valid target!")
    endif(NOT TARGET ${target_name})

    #target_include_directories(${caller_name} PUBLIC "${zq_root}/${target_path}/")
    target_link_libraries(${caller_name} ${target_name})
endfunction()

function(zq_add_module caller_name)
    foreach(target_path ${ARGN})
        __zq_add_module(${caller_name} ${target_path})
    endforeach()
endfunction()

macro(zq_add_module_auto_caller)
    zq_add_module(${PROJECT_NAME} ${ARGN})
endmacro()

macro(zq_add_library library_name)
#    project(${library_name})

    foreach(class_name ${ARGN})
        set(files_list ${files_list} "${class_name}.${zq_cpp_ext}" "${class_name}.${zq_h_ext}")
    endforeach()

    add_library("${zq_target_prefix}${library_name}" ${zq_library_type} ${files_list})
endmacro()

if (zq_test_build)
    set(zq_compiler_arguments "-Wno-old-style-cast -fno-rtti -fno-exceptions -Weverything -Wno-c++98-compat -Wno-documentation -Wno-padded -Wno-c++98-c++11-compat-pedantic -Wno-c++98-compat-pedantic")
else ()
    set(zq_compiler_arguments "-Wno-old-style-cast -nostdlib -fno-rtti -fno-exceptions -Weverything -Wno-c++98-compat -Wno-documentation -Wno-padded -Wno-c++98-c++11-compat-pedantic -Wno-c++98-compat-pedantic")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${zq_compiler_arguments}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${zq_compiler_arguments}")

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory (Base)
#add_subdirectory (ThirdParty)
add_subdirectory (Network)
add_subdirectory (Core)
zq_add_library (Main main)
add_subdirectory (ZiqeAPI)

#add_subdirectory (Worker)
#add_subdirectory (Client)
