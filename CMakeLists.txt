project(Ziqe)

cmake_minimum_required(VERSION 3.3)

# Set Ziqe variables.
set(zq_root ${CMAKE_CURRENT_SOURCE_DIR})
set(zq_library_type STATIC)
set(zq_cpp_ext cpp)
set(zq_target_prefix Ziqe_)
set(zq_h_ext hpp)

set(zq_debug ON)
set(zq_test_build ON)

# Linux | Posix | IOKit | WDK
set(zq_platform Linux)

# Combining linux headers is impossible: inline functions with linux code would not work
set(zq_cpp_compiler_arguments -std=c++14 -Wno-old-style-cast -Wno-c++98-compat -Wno-documentation -Wno-padded
    -Wno-c++98-c++11-compat-pedantic -Wno-c++98-compat-pedantic -fno-rtti -fno-exceptions
    CACHE INTERNAL "")
#                               -isystem /usr/lib/gcc/x86_64-pc-linux-gnu/6.2.1/include -I/lib/modules/4.8.13-1-ARCH/build/arch/x86/include -I/lib/modules/4.8.13-1-ARCH/build/arch/x86/include/generated/uapi -I/lib/modules/4.8.13-1-ARCH/build/arch/x86/include/generated  -I/lib/modules/4.8.13-1-ARCH/build/include -I/lib/modules/4.8.13-1-ARCH/build/arch/x86/include/uapi -I/lib/modules/4.8.13-1-ARCH/build/include/uapi -I/lib/modules/4.8.13-1-ARCH/build/include/generated/uapi -include /lib/modules/4.8.13-1-ARCH/build/include/linux/kconfig.h -D__KERNEL__
#                               -nostdinc -nostdinc++)

if (zq_test_build)
    set(zq_cpp_compiler_arguments ${zq_cpp_compiler_arguments} -DZQ_TEST_BUILD)
    enable_testing()
endif()

set(zq_include_directories ${zq_root}
    "${zq_root}/Platforms/${zq_platform}/"
    "${zq_root}/Platforms/Common/")

if(zq_test_build)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

macro(zq_library_to_target output_name)
    set(${output_name} "")

    foreach(library_name ${ARGN})
        set(${output_name} ${${output_name}} "${zq_target_prefix}${library_name}")
    endforeach()
endmacro()

macro(zq_get_target_dependencies output_var target)
    # Target is not valid
    if (NOT TARGET ${target})
        message(FATAL_ERROR "${target} is not a valid target!")
    endif()

    get_target_property(dependencies ${target} LINK_LIBRARIES)

    # If this target doesn't has any dependencies, leave this function.
    if (${dependencies} STREQUAL dependencies-NOTFOUND)
        set(${output_var} "")
    else()
        foreach(dependency ${dependencies})
            zq_get_target_dependencies(more_dependencies ${dependency})
            set(dependencies ${more_dependencies} ${dependencies})
        endforeach()

        set(${output_var} ${dependencies} PARENT_SCOPE)
    endif()
endmacro()

macro(zq_get_module_dependencies output_var target)
    zq_library_to_target(target ${target})

    zq_get_target_dependencies(${output_var} ${target})

    # Update the parent scope's output_var.
    set(${output_var} ${${output_var}} PARENT_SCOPE)
endmacro()

function(zq_add_one_depend_module caller_name target_name)
    zq_library_to_target(caller_name ${caller_name})
    zq_library_to_target(target_name ${target_name})

    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "${target_name} is not a valid target!")
    endif(NOT TARGET ${target_name})

    target_link_libraries(${caller_name} ${target_name})
endfunction()

function(zq_add_depend_modules caller_name)
    foreach(target ${ARGN})
        zq_add_one_depend_module(${caller_name} ${target})
    endforeach()
endfunction()

function(zq_add_library)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME)
    set(_MULTI_VALUE_ARGS SOURCES CLASSES INCLUDES LIBRARIES)

    cmake_parse_arguments(LIBRARY "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    if (NOT LIBRARY_NAME)
        message(FATAL_ERROR "NAME is required")
    endif ()

    foreach(file_name ${LIBRARY_SOURCES})
        list(APPEND files_list "${file_name}")
    endforeach()

    foreach(class_name ${LIBRARY_CLASSES})
        list(APPEND files_list "${class_name}.${zq_cpp_ext}" "${class_name}.${zq_h_ext}")
    endforeach()

    set (target "${zq_target_prefix}${LIBRARY_NAME}")

    add_library("${target}" ${zq_library_type} ${files_list})
    target_compile_options("${target}" PUBLIC ${zq_cpp_compiler_arguments})
    target_include_directories ("${target}" PUBLIC ${zq_include_directories} ${LIBRARY_INCLUDES})
    target_link_libraries ("${target}" ${LIBRARY_LIBRARIES})
endfunction()

macro (zq_add_test name)
    if (zq_test_build)
        set(target_name "test_${name}")
        zq_library_to_target(target_name ${target_name})

        add_library("${target_name}_lib" "${name}.${zq_cpp_ext}")
        target_compile_options("${target_name}_lib" PUBLIC ${zq_cpp_compiler_arguments})
        target_include_directories ("${target_name}_lib" PUBLIC ${zq_include_directories})

        zq_platform_add_driver (NAME "${target_name}_driver" LIBRARIES "${target_name}_lib" ${ARGN})

    #    add_executable (${target_name} "${name}.${zq_cpp_ext}")
        add_test (${target_name} "./${name}")
    #    zq_add_module(${target_name} ${arguments})
    #    target_include_directories (${target_name} PUBLIC ${zq_include_directories})
    #    target_compile_options (${target_name} PUBLIC ${zq_compiler_arguments})
    endif()
endmacro ()

# Output a string of elements, splitted by spaces.
macro(zq_list_to_string input_list output_string_name)
    string (REPLACE ";" " " "${output_string_name}" "${input_list}")
endmacro()

function(zq_apply_global_compiler_options options)
    list(APPEND zq_cpp_compiler_arguments ${options})
endfunction()

add_subdirectory (Platforms)
message(${zq_cpp_compiler_arguments})
add_subdirectory (Utils)
#add_subdirectory (ThirdParty)
#add_subdirectory (Network)
#add_subdirectory (Core)

# Build Ziqe's main driver
zq_add_library (NAME MainDriver SOURCES MainDriver.cpp)
zq_common_add_driver (NAME MainDriver LIBRARIES Ziqe_MainDriver)
