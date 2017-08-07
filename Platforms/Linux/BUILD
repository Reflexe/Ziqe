load("//BuildTools:test.bzl", 'cc_class_library')
load(':api.bzl', 'linux_lastest_path')

cc_library (
    name = 'ZqAPI',
    hdrs = [
        'ZqAPI/Memory.h', 
        'ZqAPI/SpinLock.h', 
        'ZqAPI/Mutex.h', 
        'ZqAPI/RWLock.h', 
        'ZqAPI/Socket.h', 
        'ZqAPI/Logging.h', 
        'ZqAPI/SystemCalls.h', 
        'ZqAPI/Types.h', 
        'ZqAPI/Error.h',
        'OS/C_API/LinuxUsbApi.h',
    ],

    includes = ['.'],
    visibility = ["//visibility:public"],
)

cc_class_library (
    name = 'OS',
    classes = [ 'OS/Memory',
                'OS/DriverContext',
                'OS/Types',
                'OS/UsbDeviceManager',
                'OS/UsbDevice',
    ],

    deps = [
        '//Platforms:ZqAPI',
        '//Platforms/Common:OS',
        '//Utils:Utils',
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
          #'ZqAPI/Memory.c', 
          #      ':ZqAPI/SpinLock.c', 
               # ':ZqAPI/Mutex.c',
                'OS/C_API/LinuxUsbApi.c',
                #':ZqAPI/RWLock.c', 
                #':ZqAPI/Socket.c', 
                ':ZqAPI/Logging.c',
                'ZqAPI/EntryPoints.c', 
                #':ZqAPI/SystemCalls.c'
                ],
        visibility = ["//visibility:public"],
)
                
# TODO: real rules
exports_files(["ZqAPI/Makefile",
               'Scripts/ParseUsbIDs.py',
               "Scripts/CompileLinuxModule.sh",
               ])
