load("//Platforms:api.bzl", 'zq_library_no_auto_deps')
load(':api.bzl', 'linux_lastest_path')

zq_library_no_auto_deps (
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
        'OS/C_API/LinuxUsbApi.h',
    ],
    zq_deps = ['//Platforms/Common:CppCore'],
    includes = ['.'],
    visibility = ["//visibility:public"],
)

# FIXME: we're creating all the libraries for each platform (eg. LIB_Linux, LIB_Windows)
#        but we leave the deps as it is. We need to dig into the cfg thingy in bazel.
zq_library_no_auto_deps (
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
        '//Utils:Utils',
    ],

    visibility = ["//visibility:public"],
    )

zq_library_no_auto_deps(
    name = 'PerDriver',
    zq_deps = ['//Platforms/Common:PerDriver'],
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
                'OS/C_API/LinuxUsbApi.c',
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
