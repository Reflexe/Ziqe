load("//Platforms:api.bzl", 'zq_library_for_platform')

package(default_visibility=['//visibility:public'])

PLATFORM = 'GenericUsermode'

zq_library_for_platform(
    PLATFORM,
    name='OS',
    classes = [
        'OS/DriverContext',
        'OS/UsbDeviceManager',
        'OS/UsbDevice',
        'OS/Internal/Context',
        'OS/Internal/UsbDevice'
    ],

    zq_deps = [
         '//Platforms/Common:OS',
         '//Platforms/GenericUsermode:PerDriver',
    ],

    linkopts = ['-lusb-1.0'],
)
zq_library_for_platform(
    PLATFORM,
    name='CppCore',
    hdrs = [
        'CppCore/Types.h',
        'CppCore/Logging.h',
        'CppCore/Memory.h',
    ],

    zq_deps = [
        '//Platforms/Common:CppCore',
    ],

    includes = ['.'],
)

zq_library_for_platform(
    PLATFORM,
    name='PerDriver',
    hdrs = ['PerDriver/UsbIds.hpp'],
    zq_deps = ['//Platforms/Common:PerDriver',
               '//Base:Base'],
)

filegroup(name='per_driver_srcs',
          srcs=['PerDriver/UsbIds.cpp',
                'PerDriver/UsbIds.hpp']
)

filegroup(name='main_driver_srcs',
    srcs = [
        'PerDriver/EntryPoints.cpp'
    ],
)

exports_files(["PerDriver/in/per_driver_macros.h"])
