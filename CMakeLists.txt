project(Ziqe)

cmake_minimum_required(VERSION 3.3)

# Set Ziqe variables.
set(zq_root ${CMAKE_CURRENT_SOURCE_DIR})
set(zq_library_type STATIC)
set(zq_cpp_ext cpp)
set(zq_target_prefix zq_)
set(zq_h_ext hpp)

set(zq_debug ON)
set(zq_test_build ON)

# Linux | Posix
set(zq_platform Linux)

set(zq_cpp_compiler_arguments -std=c++14 -Weverything -Wno-old-style-cast -Wno-c++98-compat -Wno-documentation -Wno-padded
                               -Wno-c++98-c++11-compat-pedantic -Wno-c++98-compat-pedantic -fno-rtti -fno-exceptions)


if (zq_test_build)
    set(zq_cpp_compiler_arguments ${zq_cpp_compiler_arguments} -DZQ_TEST_BUILD)
    enable_testing()
endif()

set(zq_include_directories ${zq_root})

if(zq_test_build)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

function(__zq_add_module caller_name target_path)
    get_filename_component(target_name ${target_path} NAME)
    set(target_name "${zq_target_prefix}${target_path}")

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
    target_compile_options("${zq_target_prefix}${library_name}" PUBLIC ${zq_cpp_compiler_arguments})
    target_include_directories ("${zq_target_prefix}${library_name}" PUBLIC ${zq_include_directories})
endmacro()

macro(zq_library_to_target output_name)
    set(${output_name} "")

    foreach(library_name ${ARGN})
        set(${output_name} ${${output_name}} "${zq_target_prefix}${library_name}")
    endforeach()
endmacro()

macro (zq_add_test name)
    if (NOT zq_test_build)
        return()
    endif ()

    set(target_name "test_${name}")
    zq_library_to_target(target_name ${target_name})

    add_library("${target_name}_lib" "${name}.${zq_cpp_ext}")
    target_compile_options("${target_name}_lib" PUBLIC ${zq_cpp_compiler_arguments})
    target_include_directories ("${target_name}_lib" PUBLIC ${zq_include_directories})

    zq_add_driver ("${target_name}_driver" "test_${name}_lib" ${ARGN})

#    add_executable (${target_name} "${name}.${zq_cpp_ext}")
    add_test (${target_name} "./${name}")
#    zq_add_module(${target_name} ${arguments})
#    target_include_directories (${target_name} PUBLIC ${zq_include_directories})
#    target_compile_options (${target_name} PUBLIC ${zq_compiler_arguments})
endmacro ()

add_subdirectory (ZiqeAPI)
add_subdirectory (Base)
#add_subdirectory (ThirdParty)
add_subdirectory (Network)
add_subdirectory (Core)

# Build Ziqe's main driver
zq_add_library(MainLibrary main)
zq_add_driver (MainDriver Base Core ZiqeProtocol Network MainLibrary)
