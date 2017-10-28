load("//Platforms:api.bzl", 'zq_library_no_auto_deps')

package(default_visibility=['//visibility:public'])

zq_library_no_auto_deps(name='OS')
zq_library_no_auto_deps(
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
zq_library_no_auto_deps(
    name='PerDriver',
    zq_deps = ['//Platforms/Common:PerDriver'],
)

filegroup(name='per_driver_srcs',
          srcs=['PerDriver/EntryPoints.c']
)
