macro(zq_get_current_linux_version output_name)
    # Run uname -r
    execute_process (COMMAND uname -r OUTPUT_VARIABLE ${output_name} OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro ()

set(zq_linux_here ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")

# SystemCalls.c Process.c
set(zq_linux_zqapi_sources Memory.c
    SpinLock.c Mutex.c EntryPoints.c Socket.c Logging.c CACHE INTERNAL "")

# Linux X-obj param would get only relative parameter; but,
# we can only get the absolute path to our library; therefore,
# we would insert the relative path of root (/) from this directory.
# (example: ../../../home/user/project/file instead of /home/user/project/file)
file(RELATIVE_PATH zq_here_to_root ${zq_linux_here} /)
set(zq_here_to_root "${zq_here_to_root}" CACHE INTERNAL "A relative path to this directory")

macro(zq_linux_get_library_path current_target output_name zq_current_bin_to_root)
    set (${output_name} "${zq_current_bin_to_root}$<TARGET_LINKER_FILE:${current_target}>")
endmacro ()

#set(zq_linux_zqapi_sources ${SOURCES} CACHE INTERNAL "The ZiqeAPI's Linux module's files")
# OS/SystemCalls.h OS/Types.h OS/Process.h

# OS/Memory.h OS/SpinLock.h OS/Mutex.h OS/RWLock.h
# OS/Socket.h OS/Logging.h
# OS/EntryPoints.h

zq_apply_global_compiler_options (ls)

# FIXME: would not work for all platform: we aren't taking care of compiler
#        arguments.
# TODO: a zq_platform_apply_compiler_config function
# Brief: Compile a linux module.
#
# @param NAME       The module name.
# @param LIBRARIES  A list of zq libraries to link this module to.
# @param SOURCES    A list of .c files to compile as a part of the module.
# @param LINUX_VERSION  A linux version to compile this module against.
# @param SOURCES_PATH   Path to the files in sources (in most cases, ${CMAKE_CURRENT_SOURCE_DIR})
# @param INCLUDE_PATHS  Paths to be added as a include directory.
# @param DEPENDS        Ziqe targets the module depends on.
function(zq_add_linux_module)
    # Parse the arguments.
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME LINUX_VERSION SOURCES_PATH)
    set(_MULTI_VALUE_ARGS LIBRARIES SOURCES INCLUDE_PATHS DEPENDS)

    cmake_parse_arguments(MODULE "${_OPTIONS_ARGS}"
                                 "${_ONE_VALUE_ARGS}"
                                 "${_MULTI_VALUE_ARGS}"
                                 ${ARGN})

    if (NOT MODULE_SOURCES)
        message(FATAL_ERROR "Not module sources")
    endif()

    set (MODULE_OUTPUT_DIRECTIRY "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_KbuildOutput")

    # Create the output directory
    file(MAKE_DIRECTORY "${MODULE_OUTPUT_DIRECTIRY}")

    foreach(file ${MODULE_SOURCES})
        # Change each source file to a full path to the file.
        list(APPEND FULL_PATH_SOURCES "${MODULE_SOURCES_PATH}/${file}")

        # Copy each file to the output directory.
        configure_file ("${MODULE_SOURCES_PATH}/${file}"
                        "${MODULE_OUTPUT_DIRECTIRY}/${file}"
                        COPYONLY)
    endforeach()

    # Make include directories compiler arguments.
    foreach(include_directory ${MODULE_INCLUDE_PATHS})
        list(APPEND MODULE_COMPILER_ARGUMENTS "-I${include_directory}")
    endforeach()

    # Get all of these libraries' dependencies.
    foreach(library ${MODULE_LIBRARIES})
        zq_get_target_dependencies (library_dependencies ${library})
        list(APPEND libraries ${library_dependencies})
    endforeach()

    # Convert all of the libraries to valid linux module paths (relative paths from
    # the build).
    file(RELATIVE_PATH output_dir_to_root ${MODULE_OUTPUT_DIRECTIRY} /)
    foreach(library ${MODULE_LIBRARIES})
        zq_linux_get_library_path (${library} library_path "${output_dir_to_root}")

        # Add this library to the objects.
        list(APPEND MODULE_OBJECTS ${library_path})
    endforeach()

    # Convert these lists to strings.
    zq_list_to_string("${MODULE_OBJECTS}" MODULE_STRING_OBJECTS)
    zq_list_to_string("${MODULE_SOURCES}" MODULE_STRING_SOURCES)
    zq_list_to_string("${MODULE_COMPILER_ARGUMENTS}" MODULE_STRING_COMPILER_ARGUMENTS)

    add_custom_target ("${zq_target_prefix}driver_${MODULE_NAME}"
        ALL     # Be a default target.
        COMMAND "${zq_linux_here}/CompileLinuxModule.sh"
                # Define the module's sources.
                "${MODULE_STRING_SOURCES}"
                # Define the project's objects to be linked to the module.
                "${MODULE_STRING_OBJECTS}"
                # Define the path to the makefile for this module.
                "${zq_linux_here}/Makefile"
                # Define the linux version for this module.
                "${MODULE_LINUX_VERSION}"
                # Define the output directory.
                "${MODULE_OUTPUT_DIRECTIRY}"
                # Define the module name.
                "${MODULE_NAME}"
                # Define extra compiler arguments
                "${MODULE_STRING_COMPILER_ARGUMENTS}"
        SOURCES ${FULL_PATH_SOURCES} "${zq_linux_here}/Makefile" "${zq_linux_here}/CompileLinuxModule.sh"
        WORKING_DIRECTORY "${MODULE_OUTPUT_DIRECTIRY}"
        COMMENT "Building Linux module: ${MODULE_NAME}"
        DEPENDS ${MODULE_LIBRARIES} ${FULL_PATH_SOURCES} ${MODULE_DEPENDS})
endfunction ()
