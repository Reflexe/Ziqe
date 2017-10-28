load("//Platforms:api.bzl", "zq_library_no_auto_deps")
package(default_visibility = ['//Platforms:__subpackages__'])

zq_library_no_auto_deps (
    name = 'OS',
    classes = [
        'OS/IDevice',
        'OS/IDeviceManager',
        'OS/ILoadableModule',
    ],
    zq_deps = [
        '//Utils:Utils',
    ],

    includes = ['.'],
)    

zq_library_no_auto_deps(
    name = 'CppCore',

    hdrs = [
        'CppCore/Macros.h',
    ],

    includes = ['.'],
)

zq_library_no_auto_deps(
    name = 'PerDriver',

    hdrs = [
        'PerDriver/EntryPoints.h',
    ],

    includes = ['.'],
)

exports_files(["PerDriver/in/config.h"])
