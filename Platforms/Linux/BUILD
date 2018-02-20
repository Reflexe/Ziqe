load("//Platforms:api.bzl", 'zq_library_for_platform')
load(':api.bzl', 'linux_lastest_path')

PLATFORM = 'Linux'

zq_library_for_platform (
    PLATFORM,
    name = 'CppCore',
    hdrs = [
        'CppCore/Memory.h', 
        'CppCore/SpinLock.h', 
        'CppCore/Mutex.h', 
        'CppCore/RWLock.h', 
        'CppCore/Socket.h', 
        'CppCore/Logging.h', 
        'CppCore/SystemCalls.h', 
        'CppCore/Types.h', 
        'CppCore/Error.h',
        'PerDriver/C_API/LinuxUsbApi.h',
    ],
    zq_deps = ['//Platforms/Common:CppCore'],
    includes = ['.'],
    visibility = ["//visibility:public"],
)

zq_library_for_platform (
    PLATFORM,
    name = 'OS',
    classes = [ 'OS/Memory',
                'OS/DriverContext',
                'OS/Types',
                'OS/UsbDeviceManager',
                'OS/UsbDevice',
                'OS/UsbInterface',
                'OS/UsbInEndpoint',
                'OS/UsbOutEndpoint',
    ],

    zq_deps = [
        ':CppCore',
        '//Platforms/Common:OS',
        '//Base:Base',
    ],

    visibility = ["//visibility:public"],
    )

zq_library_for_platform(
    PLATFORM,
    name = 'PerDriver',
    zq_deps = ['//Platforms/Common:PerDriver'],
    visibility = ["//visibility:public"],
)

filegroup(
    name = 'linux_per_driver_srcs',
    srcs = [
        'PerDriver/CppEntryPoints.cpp',
    ],
    visibility = ["//visibility:public"],
)

## TODO: filegroup for each linux headers and a macro
filegroup(name = '_linux_build_headers', 
          srcs=glob(include=['LinuxHeaders/**'], exclude_directories=0),
          visibility = ["//visibility:public"],
          )

exports_files ([linux_lastest_path + '/build'])

filegroup(name = '_linux_build_srcs', 
          srcs = [
          #'CppCore/Memory.c', 
          #      ':CppCore/SpinLock.c', 
               # ':CppCore/Mutex.c',
                'PerDriver/C_API/LinuxUsbApi.c',
                #':CppCore/RWLock.c', 
                #':CppCore/Socket.c', 
                ':CppCore/Logging.c',
                'PerDriver/EntryPoints.c',
                #':CppCore/SystemCalls.c'
                ],
        visibility = ["//visibility:public"],
)
                
# TODO: real rules
exports_files(["CppCore/Makefile",
               'Scripts/ParseUsbIDs.py',
               "Scripts/CompileLinuxModule.sh",
               ])
